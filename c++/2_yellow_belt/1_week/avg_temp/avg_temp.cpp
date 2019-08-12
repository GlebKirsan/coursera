#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    size_t n;
    cin >> n;
    vector<int64_t> temps(n);
    for (auto &t : temps) {
        cin >> t;
    }
    int64_t sum = 0;
    for (const auto t : temps) {
        sum += t;
    }
    int64_t avg = sum / static_cast<int>(temps.size());
    cout << count_if(begin(temps), end(temps), [avg](int64_t x) {
        return x > avg;
    }) << endl;
    for(size_t i = 0; i < temps.size(); ++i) {
        if (temps[i] > avg){
            cout << i << " ";
        }
    }
}