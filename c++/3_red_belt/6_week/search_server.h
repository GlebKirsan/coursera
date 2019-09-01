#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>

using namespace std;

class Statistic {
    struct Stat {
        size_t id;
        map<string_view, size_t> word_occurrences;
    };

public:

    Statistic() {
        documents.reserve(MAX_DOCUMENTS);
    }

    const Stat& GetDocument(size_t id) const {
        return documents[id];
    }

    const vector<Stat>& GetDocuments() const {
        return documents;
    }

    void Add(vector<pair<size_t, string>>&& lines);

private:
    static const size_t MAX_DOCUMENTS = 50'000;

    vector<Stat> documents;
    map<string, string_view> word_to_view;
};

class SearchServer {
public:
    SearchServer() = default;

    explicit SearchServer(istream& document_input);

    void UpdateDocumentBase(istream& document_input);

    void ProcessSingleQuerie(const string& query_input, ostream& search_results_output);

    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    Statistic statistic;
};
