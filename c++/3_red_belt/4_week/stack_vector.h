#pragma once

#include <stdexcept>
#include <array>
#include <iostream>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0){
      if (a_size > N){
          throw invalid_argument("Too much size");
      }
      size = a_size;
  }

  T& operator[](size_t index){
      return my_array[index];
  }
  const T& operator[](size_t index) const {
      return my_array.at(index);
  }

  typename std::array<T, N>::iterator begin() {
      return my_array.begin();
  }
  typename std::array<T, N>::iterator end() {
      return begin() + size;
  }
  typename std::array<T, N>::const_iterator begin() const {
      return my_array.cbegin();
  }
  typename std::array<T, N>::const_iterator end() const {
      return begin() + size;
  }

  size_t Size() const {
      return size;
  }
  size_t Capacity() const {
      return N;
  }

  void PushBack(const T& value){
      if (size == N){
          throw overflow_error("Size == N");
      }
      my_array[size++] = value;
  }
  T PopBack() {
      if (size == 0){
          throw underflow_error("Size == 0");
      }
      return my_array[--size];
  }

private:

  std::array<T, N> my_array;
  size_t size = 0;
};

