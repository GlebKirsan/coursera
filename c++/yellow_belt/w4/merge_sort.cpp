#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <set>
using namespace std;

//template <typename RandomIt>
//void MergeSort(RandomIt range_begin, RandomIt range_end){
//    int diff = range_end - range_begin;
//    if (diff < 2){
//        return;
//    }
//    using RIt = vector<typename RandomIt::value_type>;
//    RIt elements(range_begin, range_end);
//    RandomIt middle = diff / 2 + begin(elements);
//    MergeSort(begin(elements), middle);
//    MergeSort(middle, end(elements));
//    merge(begin(elements), middle, middle, end(elements), range_begin);
//}

//
//
//int main() {
//    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
//    MergeSort(begin(v), end(v));
//    for (int x : v) {
//        cout << x << " ";
//    }
//    cout << endl;
//    return 0;
//}

//template <typename RandomIt>
//void MergeSort(RandomIt range_begin, RandomIt range_end){
//    int diff = range_end - range_begin;
//    if (diff < 2){
//        return;
//    }
//    using RIt = vector<typename RandomIt::value_type>;
//    RIt elems(range_begin, range_end);
//    RandomIt part1 = diff/3 + range_begin;
//    RandomIt part2 = diff*2/3 + range_begin;
//    MergeSort(range_begin, part1);
//    MergeSort(part1, part2);
//    MergeSort(part2, range_end);
//
//    RIt tmp;
//    merge(range_begin, part1, part1, part2, back_inserter(tmp));
//    merge(begin(tmp), end(tmp), part2, range_end, range_begin);
//}

//int main() {
//    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
//    MergeSort(begin(v), end(v));
//    for (int x : v) {
//        cout << x << " ";
//    }
//    cout << endl;
//    return 0;
//}
