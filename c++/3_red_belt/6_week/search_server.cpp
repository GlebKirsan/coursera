#include "search_server.h"
#include "iterator_range.h"
#include "parse.h"

#include <string_view>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <list>
#include <future>

vector<string> SplitIntoWords(const string &line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

vector<string_view> SplitIntoWords(const string_view line) {
    return SplitBy(line, ' ');
}

SearchServer::SearchServer(istream &document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream &document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document);) {
        new_index.Add(move(current_document));
    }

    sync.GetAccess().ref_to_value = move(new_index);
}

ostringstream ProcessSingleThread(vector<string> lines,
                                  Synchronized<InvertedIndex> &sync) {
    size_t count_size = sync.GetAccess().ref_to_value.GetDocumentSize();
    vector<pair<size_t, size_t>> docid_count(count_size, {0, 0});
    ostringstream answer;
    for (auto &current_query : lines) {
        for (const auto &word : SplitIntoWords(string_view(current_query))) {
            for (const pair<size_t, size_t> &docid : sync.GetAccess().ref_to_value.Lookup(word)) {
                docid_count[docid.first].second += docid.second;
                docid_count[docid.first].first = docid.first;
            }
        }

        size_t min_to_sort = min<size_t>(5, docid_count.size());
        partial_sort(begin(docid_count),
                     next(begin(docid_count), min_to_sort),
                     end(docid_count),
                     [](const pair<size_t, size_t> &lhs, const pair<size_t, size_t> &rhs) {
                         int64_t lhs_docid = -static_cast<int64_t>(lhs.first);
                         int64_t rhs_docid = -static_cast<int64_t>(rhs.first);
                         return tie(lhs.second, lhs_docid) > tie(rhs.second, rhs_docid);
                     });

        answer << current_query << ':';
        for (auto&[docid, hitcount] : Head(docid_count, 5)) {
            if (hitcount == 0)
                continue;

            answer << " {"
                   << "docid: " << docid << ", "
                   << "hitcount: " << hitcount << '}';
        }
        answer << '\n';
        docid_count.assign(count_size, {0, 0});
    }
    return move(answer);
}

void SearchServer::AddQueriesStream(
        istream &query_input,
        ostream &search_results_output) {
    vector<string> inputs;

    for (string current_query; getline(query_input, current_query);) {
        inputs.push_back(current_query);
    }
    vector<future<ostringstream>> futures;
    vector<string> lines;
    for (auto &current_query : Paginate(inputs, (inputs.size() + 5) / 6)) {
        futures.push_back(
                async(ProcessSingleThread, move(vector<string>{current_query.begin(), current_query.end()}), ref(sync))
        );
    }
    for (auto &answer : futures) {
        search_results_output << answer.get().str();
    }
}

void InvertedIndex::Add(string document) {
    docs.push(move(document));

    const size_t docid = docs.size() - 1;
    for (const auto &word : SplitIntoWords(string_view(docs.back()))) {
        vector<pair<size_t, size_t>> &docs = index[word];
        if (!docs.empty() and docs.back().first == docid) {
            ++docs.back().second;
        } else {
            docs.push_back({docid, 1});
        }
    }
}

const vector<pair<size_t, size_t>> &InvertedIndex::Lookup(const string_view &word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return empty_vector;
    }
}
