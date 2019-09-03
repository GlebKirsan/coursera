#include "test_runner.h"
#include "profile.h"


#include <future>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <deque>

using namespace std;


template<typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) :
            _begin(begin),
            _end(end),
            size_(std::distance(begin, end)) {}

    Iterator begin() const {
        return _begin;
    }

    Iterator end() const {
        return _end;
    }

    Iterator begin() {
        return _begin;
    }

    Iterator end() {
        return _end;
    }

    size_t size() const {
        return size;
    }

private:
    Iterator _begin;
    Iterator _end;
    size_t size_;
};

template<typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (size_t left = distance(begin, end); left > 0;) {
            size_t current_page_size = min(page_size, left);
            Iterator page_end = next(begin, current_page_size);
            pages.push_back({begin, page_end});
            begin = page_end;
            left -= current_page_size;
        }
    }

    auto begin() const {
        return pages.begin();
    }

    auto end() const {
        return pages.end();
    }

    auto begin() {
        return pages.begin();
    }

    auto end() {
        return pages.end();
    }

    size_t size() const {
        return pages.size();
    }

private:
    vector<IteratorRange<Iterator>> pages;
};

template<class Container>
auto Paginate(Container &container, size_t page_size) {
    return Paginator(container.begin(), container.end(), page_size);
}

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats &other) {
        for (const auto&[word, number_of_occurrences] : other.word_frequences) {
            word_frequences[word] += number_of_occurrences;
        }
    }
};

Stats ExploreLine(const set<string> &key_words, const string &line) {
    Stats line_occurrences;
    stringstream ss{line};
    for (string word; ss >> word;) {
        if (key_words.count(word) > 0) {
            ++line_occurrences.word_frequences[word];
        }
    }
    return line_occurrences;
}

Stats ExploreKeyWordsSingleThread(
        const set<string> &key_words, istream &input
) {
    Stats result;
    for (string line; getline(input, line);) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
    vector<future<Stats>> futures;
    vector<string> lines;
    for (string line; getline(input, line);) {
        lines.push_back(line);
    }

    deque<stringstream> storage;
    for (auto &page : Paginate(lines, (lines.size() + 3) / 4)) {
        stringstream in;
        for (string &line : page) {
            in << line << '\n';
        }
        storage.push_back(move(in));
        futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(storage.back())));
    }

    Stats statistic;
    for(future<Stats>& stat : futures){
        statistic += stat.get();
    }
    return statistic;
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks",  2},
            {"sucks",  1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}
