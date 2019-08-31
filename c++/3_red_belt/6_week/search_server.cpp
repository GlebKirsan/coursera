#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <future>

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

vector<string> SplitIntoWords(string&& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    Statistic new_statistic;
    const int MAX_LINES = 5000;

    vector<future<void>> futures;
    vector<pair<size_t, string>> lines;
    lines.reserve(MAX_LINES);
    size_t i = 0;
    for (string current_document; getline(document_input, current_document); ++i) {
        lines.push_back({i, current_document});
        if ((i + 1) % MAX_LINES == 0) {
            futures.push_back(async([&new_statistic, &lines] {
                new_statistic.Add(move(lines));
            }));
        }
    }
    futures.clear();
    statistic = move(new_statistic);
}

void SearchServer::ProcessSingleQuerie(const string& current_query,
                                       ostream& search_results_output) {

}

void SearchServer::ProcessLines(vector<string>&& lines,
                                ostream& search_results_output) {

}

void SearchServer::AddQueriesStream(
        istream& query_input, ostream& search_results_output) {
    const int MAX_LINES = 5000;

    vector<future<void>> futures;
    vector<string> lines;
    lines.reserve(MAX_LINES);

    size_t i = 0;
    for (string current_query; getline(query_input, current_query); ++i) {
        lines.push_back(current_query);
        if ((i + 1) % MAX_LINES == 0) {
            futures.push_back(async([&lines, &search_results_output] {
                ProcessLines(lines, );
            }));
        }
    }
}

void Statistic::Add(vector<pair<size_t, string>>&& lines) {
    for (auto&[id, sentence] : lines) {
        auto words = SplitIntoWords(move(sentence));
        map<string_view, size_t> occurrences;
        for (auto& word : words) {
            if (word_to_view.count(word) == 0) {
                word_to_view[word] = word;
            }
            string_view word_view = word_to_view[word];
            ++occurrences[word_view];
        }
        documents.push_back(Stat{id, move(occurrences)});
    }
}

