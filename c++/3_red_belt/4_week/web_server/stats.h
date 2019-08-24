#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <set>
using namespace std;

class Stats {
public:
  Stats();
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;

private:
  set<string> unique_methods = {"GET", "PUT", "POST", "DELETE", "UNKNOWN"};
  set<string> unique_uris = {"/", "/order", "/product", "/basket", "/help", "unknown"};
  string str_unknown{"unknown"};
  string str_UNKNOWN{"UNKNOWN"};
  string_view UNKNOWN;
  string_view unknown;

  map<string_view, int> methods_stats;
  map<string_view, int> uri_stats;
};

HttpRequest ParseRequest(string_view line);
