#include <test_runner.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
class PriorityCollection {
public:
    using Id = int64_t;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        int64_t id = objects.size();
        objects.push_back({move(object)});
        indices.insert({0, id});
        return id;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        while (range_begin != range_end) {
            *ids_begin++ = Add(move(*range_begin++));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return id >= 0 && id < objects.size() &&
               objects[id].priority != NONE_PRIORITY;
    }

    // Получить объект по идентификатору
    const T &Get(Id id) const {
        return objects[id].value;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        int64_t previous_priority = objects[id].priority;
        indices.erase({previous_priority, id});
        ++objects[id].priority;
        indices.insert({previous_priority + 1, id});
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const {
        const auto &item = objects[prev(end(indices))->second];
        return {item.value, item.priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        const auto iterator = prev(end(indices));
        auto& item = objects[iterator->second];
        indices.erase(iterator);
        const int priority = item.priority;
        item.priority = NONE_PRIORITY;
        return {move(item.value), priority};
    }

private:
    struct Item {
        T value;
        int64_t priority = 0;
    };
    static const int NONE_PRIORITY = -1;

    vector<Item> objects;
    set<pair<int, Id>> indices;
};


class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable &) = delete;

    StringNonCopyable(StringNonCopyable &&) = default;

    StringNonCopyable &operator=(const StringNonCopyable &) = delete;

    StringNonCopyable &operator=(StringNonCopyable &&) = default;
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

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
