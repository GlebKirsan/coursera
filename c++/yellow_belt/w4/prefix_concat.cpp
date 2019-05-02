#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <typename RandomIt, typename str>
pair<RandomIt, RandomIt> FindStartsWith(
        RandomIt range_begin, RandomIt range_end,
        const str prefix){
    const string pr{prefix};
    auto first = lower_bound(range_begin, range_end, pr, []
    (const string &word, const string &pref){
        size_t min_size = min(word.length(), pref.length());
        return word.substr(0, min_size) < pref;
    });
    auto lastt = upper_bound(range_begin, range_end, pr, []
            (const string &pref, const string &word){
        size_t min_size = min(word.length(), pref.length());
        return word.substr(0, min_size) > pref;
    });
    return {first, lastt};
}
