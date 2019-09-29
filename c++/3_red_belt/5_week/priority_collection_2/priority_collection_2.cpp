#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <list>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
struct TimedItem {
    T data;
    int priority = -1;
    int time_of_insertion = -1;

    TimedItem(T object, int priority_, int time_of_insertion_) :
            data{move(object)},
            priority{priority_},
            time_of_insertion{time_of_insertion_} {}

    bool operator<(const TimedItem& other) const {
        return tie(priority, time_of_insertion, data) <
               tie(other.priority, other.time_of_insertion, other.data);
    }
};

template<typename T>
using Id = typename set<TimedItem<T>>::const_iterator;

template<typename T>
class PriorityCollection {

public:
    using Id = typename set<TimedItem<T>>::const_iterator;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        TimedItem to_insert{move(object), 0, time_of_insertion++};
        return collection.insert(move(to_insert)).first;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        for (auto it = range_begin; it != range_end; ++it, ++ids_begin) {
            *ids_begin = Add(move(*it));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return collection.find(*id) != end(collection);
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return id->data;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        TimedItem temp = move(collection.extract(id).value());
        ++temp.priority;
        id = collection.insert(move(temp)).first;
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        const T& element = rbegin(collection)->data;
        int priority = rbegin(collection)->priority;
        return {element, priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        TimedItem temp = move(collection.extract(prev(end(collection))).value());
        T element = move(temp.data);
        int priority = temp.priority;
        return {move(element), priority};
    }

private:
    int time_of_insertion = 0;
    set<TimedItem<T>> collection;
};

class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable&) = delete;

    StringNonCopyable(StringNonCopyable&&) = default;

    StringNonCopyable& operator=(const StringNonCopyable&) = delete;

    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

void TestValidation() {
    PriorityCollection<StringNonCopyable> strings;
    PriorityCollection<StringNonCopyable> another_strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    const auto black_id = another_strings.Add("black");
    {
        ASSERT_EQUAL(strings.IsValid(white_id), true);
        ASSERT_EQUAL(another_strings.IsValid(black_id), true);
        ASSERT_EQUAL(strings.IsValid(black_id), false);
    }
}

void TestMultipleAdd() {
    PriorityCollection<StringNonCopyable> strings;
    vector<StringNonCopyable> test;
    test.emplace_back("white");
    test.emplace_back("yellow");
    test.emplace_back("red");

    vector<Id<StringNonCopyable>> a;
    strings.Add(begin(test), end(test), back_inserter(a));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestValidation);
    return 0;
}
