#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <deque>
using namespace std;

int main(){
    string x;
    cin >> x;
    int N;
    cin >> N;
    if (N == 0){
        cout << x << endl;
        exit(0);
    }

    deque<string> q;
    q.push_back(x);
    string op, val;
    string tmp="?";
    for(int i = 0; i < N; ++i){
        cin >> op >> val;
        if ((op == "*" || op == "/") &&
                (tmp == "+" || tmp == "-")){
            q.push_front("(");
            q.push_back(")");
        }
        q.push_back(" ");
        q.push_back(op);
        q.push_back(" ");
        q.push_back(val);
        tmp = op;
    }
    for(const auto &symb : q){
        cout << symb;
    }
}