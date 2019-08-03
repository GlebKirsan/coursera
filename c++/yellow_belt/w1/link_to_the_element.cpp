#include <iostream>
#include <map>

using namespace std;

template <typename T, typename V>
V &GetRefStrict(map<T, V> &m, T el){
    if (m.count(el) == 0){
        throw runtime_error("");
    }
    return m[el];
}

int main(){
    map<int, string> m = {{0, "value"}};
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue
}