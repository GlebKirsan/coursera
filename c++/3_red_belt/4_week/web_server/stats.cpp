#include "stats.h"

#include <algorithm>
#include <iostream>

Stats::Stats(){
    for(auto& method : unique_methods){
        methods_stats[method] = 0;
    }
    UNKNOWN = str_UNKNOWN; 
    for(auto& uri : unique_uris){
        uri_stats[uri] = 0;
    }
    unknown = str_unknown;
}

const map<string_view, int>& Stats::GetMethodStats() const {
    return methods_stats;
}

const map<string_view, int>& Stats::GetUriStats() const {
    return uri_stats;
}

HttpRequest ParseRequest(string_view line) {
    int leading_whitespace = line.find_first_not_of(' ');
    line.remove_prefix(leading_whitespace);
    HttpRequest parsed_request;

    auto whitespace_pos = line.find(' ');
    parsed_request.method = line.substr(0, whitespace_pos);
    line.remove_prefix(whitespace_pos + 1);

    whitespace_pos = line.find(' ');
    parsed_request.uri = line.substr(0, whitespace_pos);
    line.remove_prefix(whitespace_pos + 1);

    auto end_of_protocol = line.find_last_not_of(' ');
    parsed_request.protocol = line.substr(0, end_of_protocol+1);
    return parsed_request;
}

void Stats::AddUri(string_view uri) {
    string_view field = uri_stats.count(uri) > 0 ?
        uri :
        unknown;
    ++uri_stats[field];
}

void Stats::AddMethod(string_view method) {
    string_view field = methods_stats.count(method) > 0 ?
        method :
        UNKNOWN;
    ++methods_stats[field];
}