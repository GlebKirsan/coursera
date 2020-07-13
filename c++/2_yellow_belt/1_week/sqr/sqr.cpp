#include <iostream>
#include <vector>
#include <map>

using namespace std;


template<typename T> T Sqr(T value);
template<typename T> vector<T> Sqr(const vector<T>& value);
template<typename K, typename V> map<K, V> Sqr(const map<K, V>& value);
template<typename K, typename V> pair<K, V> Sqr(const pair<K, V>& value);

template<typename T>
T Sqr(T value) {
    return value * value;
}

template<typename T>
vector<T> Sqr(const vector<T>& value) {
    vector<T> result;
    for (const T& x : value) {
        result.push_back(Sqr(x));
    }

    return result;
}

template<typename K, typename V>
map<K, V> Sqr(const map<K, V>& value) {
    map<K, V> result;
    for (const auto& element : value) {
        result[element.first] = Sqr(element.second);
    }
    return result;
}

template<typename K, typename V>
pair<K, V> Sqr(const pair<K, V>& value) {
    return {Sqr(value.first), Sqr(value.second)};
}

int main(){
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v))
    {
        cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
        {4, {2, 2}},
        {7, {4, 3}}};
    cout << "map of pairs:" << endl;
    for (const auto &x : Sqr(map_of_pairs))
    {
        cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
    system("pause");
}