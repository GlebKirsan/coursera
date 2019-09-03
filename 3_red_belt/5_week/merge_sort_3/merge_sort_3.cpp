#include "test_runner.h"
#include <algorithm>
#include <utility>
#include <iterator>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  // Напишите реализацию функции,
  // не копируя сортируемые элементы
  auto dist = std::distance(range_begin, range_end);
  if (dist < 2) {
    return;
  }
  using val = typename RandomIt::value_type;

  // vector<val> elements;
  // elements.reserve(dist);
  // move(range_begin, range_end, back_inserter(elements));

  auto addition = dist / 3;
  
  auto one_third = range_begin + addition;
  auto two_third = range_begin + addition * 2;

  MergeSort(range_begin, one_third);
  MergeSort(one_third, two_third);
  MergeSort(two_third, range_end);

  vector<val> wo_right;
  #define mmi make_move_iterator
  merge(mmi(range_begin), mmi(one_third), 
        mmi(one_third), mmi(two_third),
        back_inserter(wo_right));
        
  merge(mmi(begin(wo_right)), mmi(end(wo_right)), 
        mmi(two_third), mmi(range_end),
        range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
