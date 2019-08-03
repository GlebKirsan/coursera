#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <set>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> permutation(n * 1ul);
    iota(begin(permutation), end(permutation), 1);
    reverse(begin(permutation), end(permutation));
    do {
        for (int num : permutation) {
            cout << num << ' ';
        }
        cout << endl;
    } while (prev_permutation(permutation.begin(), permutation.end()));

    return 0;
}