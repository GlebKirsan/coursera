#include "test_runner.h"
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target){
      auto source_it = words.insert(string(source));
      auto target_it = words.insert(string(target));
      source = *source_it.first;
      target = *target_it.first;
      forw_map[source] = target;
      back_map[target] = source;
  }
  string_view TranslateForward(string_view source) const {
      return forw_map.count(source) > 0 ?
          forw_map.at(source) :
          "";
  }
  string_view TranslateBackward(string_view target) const {
      return back_map.count(target) > 0 ?
          back_map.at(target) :
          "";
  }

private:
  set<string> words;
  map<string_view, string_view> forw_map;
  map<string_view, string_view> back_map;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
