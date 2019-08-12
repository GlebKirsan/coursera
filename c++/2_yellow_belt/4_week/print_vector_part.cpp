#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

void PrintVectorPart(const vector<int> &a){
    auto it = find(begin(a), end(a),
            [](const int num){
        return num < 0;
    });
    while(it != begin(a)){
        it--;
        cout << *it << " ";
    }
}

int main() {
    PrintVectorPart({6, 1, 8, -5, 4});
    cout << endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    cout << endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    cout << endl;
    return 0;
}
