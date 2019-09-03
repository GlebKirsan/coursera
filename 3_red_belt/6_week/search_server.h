#pragma once

#include <istream>
#include <ostream>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <mutex>

using namespace std;

class InvertedIndex {
public:
    void Add(string document);

    const vector<pair<size_t, size_t>> &Lookup(const string_view &word) const;

    size_t GetDocumentSize() const {
        return docs.size();
    }

private:
    vector<pair<size_t, size_t>> empty_vector;
    map<string_view, vector<pair<size_t, size_t>>> index;
    queue<string> docs;
};

template<typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()):
            value(move(initial)){}

    struct Access {
        T &ref_to_value;
        lock_guard<mutex> guard;
    };

    Access GetAccess() {
        return {value, lock_guard(m)};
    }

private:
    T value;
    mutex m;
};

class SearchServer {
public:
    SearchServer() = default;

    explicit SearchServer(istream &document_input);

    void UpdateDocumentBase(istream &document_input);

    void AddQueriesStream(istream &query_input, ostream &search_results_output);

private:
    Synchronized<InvertedIndex> sync;
};
