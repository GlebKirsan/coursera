#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border) {
    // Начнём итерироваться по множеству
    auto it = begin(elements);
    // Цель — найти первый элемент, больший border
    // Если итератор не достиг конца и указывает не туда, двигаем
    while (it != end(elements) && *it <= border) {
        ++it;
    }
    // Возвращаем вектор, созданный из элементов множества, начиная с it
    return {it, end(elements)};
}

//int main() {
//    for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
//        cout << x << " ";
//    }
//    cout << endl;
//
//    string to_find = "Python";
//    cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
//    return 0;
//}
