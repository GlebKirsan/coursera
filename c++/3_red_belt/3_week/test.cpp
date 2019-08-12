#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <iostream>

using namespace std;

int main(){
    int c = 89;
    int* d = &c;
    int* e = d + 3;
    cout << d << endl
        << e << endl
        << c << endl;
}
