#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>

#define push_back(word) insert(word)
#define vector set
#define find(a, b, c) dict.find(c)
#define sort(a, b) ;
#define unique(a, b) dict.end()

using namespace std;

class Learner {
private:
    set<string> dict;

public:
    int Learn(const vector<string> &words) {
        int newWords = 0;
        for (const auto &word : words) {
            if (find(dict.begin(), dict.end(), word) == dict.end()) {
                ++newWords;
                dict.push_back(word);
            }
        }
        return newWords;
    }

    vector<string> KnownWords() {
        sort(dict.begin(), dict.end());
        dict.erase(unique(dict.begin(), dict.end()), dict.end());
        return dict;
    }
};