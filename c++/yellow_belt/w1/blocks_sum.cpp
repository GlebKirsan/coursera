#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main(){
    uint64_t sum = 0;
    uint64_t R = 0;
    int n = 0;
    cin >> n >> R;
    for(int i = 0; i < n; ++i){
        uint16_t h, w, d;
        cin >> h >> w >> d;
        sum += R*h*w*d;
    }
    cout << sum << endl;
}