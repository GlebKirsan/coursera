#include "test_runner.h"
#include <vector>
#include <future>
#include <numeric>
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
            : first(begin)
            , last(end)
            , size_(distance(first, last))
    {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

    size_t size() const {
        return size_;
    }

private:
    Iterator first, last;
    size_t size_;
};

template <typename Iterator>
class Paginator {
private:
    vector<IteratorRange<Iterator>> pages;

public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (size_t left = distance(begin, end); left > 0; ) {
            size_t current_page_size = min(page_size, left);
            Iterator current_page_end = next(begin, current_page_size);
            pages.push_back({begin, current_page_end});

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    auto begin() const {
        return pages.begin();
    }

    auto end() const {
        return pages.end();
    }

    size_t size() const {
        return pages.size();
    }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template <typename C>
auto Paginate(C& c, double page_size) {
    return Paginator(begin(c), end(c), static_cast<size_t>(page_size));
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  vector<future<int64_t>> futures;
  for (auto page : Paginate(matrix, (matrix.size() + 3) / 4)) {
    futures.push_back(async([=] { 
        int64_t sum = 0;
        for(auto it = page.begin(); it != page.end(); ++it){
          sum += accumulate(it->begin(), it->end(), 0);
        }
        return sum; 
      })
    );
  }

  int64_t sum = 0;
  for(future<int64_t>& part_of_sum : futures){
    sum += part_of_sum.get();
  }
  return sum;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 78);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}