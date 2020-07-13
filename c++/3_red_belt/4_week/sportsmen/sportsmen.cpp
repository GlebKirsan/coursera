#include <iostream>
#include <vector>
#include <list>
using namespace std;

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(nullptr);
    const int MAX_SP_MAN = 100'000;
    list<int> sportsmen;
    vector<list<int>::iterator> pos_in_list(MAX_SP_MAN, end(sportsmen));

    int n, number, prev_sportsmen;
    cin >> n;
    for(int i = 0; i < n; ++i){
        cin >> number >> prev_sportsmen;
        if (pos_in_list[prev_sportsmen] == end(sportsmen)){
            sportsmen.push_back(number);
            pos_in_list[number] = prev(end(sportsmen));
        } else {
            pos_in_list[number] = sportsmen.insert(
                    pos_in_list[prev_sportsmen], number);
                    
        }
    }
    for(const auto& s_man : sportsmen){
        cout << s_man << ' ';
    }
    cout << endl;
}
