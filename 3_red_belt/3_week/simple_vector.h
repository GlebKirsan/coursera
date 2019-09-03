#pragma once

#include <cstdlib>
#include <iostream>

template <typename T>
class SimpleVector {
public:
  SimpleVector(){
      data = nullptr;
      end_ = nullptr;
      size_ = 0;
      cap = 0;
  }

  explicit SimpleVector(size_t size){
      data = new T[size];
      end_ = data + size;
      size_ = size;
      cap = size_;
  }

  ~SimpleVector() {
      delete[] data;
  }

  T& operator[](size_t index){
      return data[index];
  }

  T* begin(){ return data; }
  T* end(){ return end_; }

  size_t Size() const { return size_; }
  size_t Capacity() const { return cap; }
  void PushBack(const T& value){
      if (size_ == cap){
          Resize();
      }
     
      data[size_] = value;
      size_ += 1;
      end_ = data + size_;
  }

private:
  void Resize(){
      cap = cap == 0 ? cap + 1 : 2*cap;
      SimpleVector<T> tmp(size_);
      for(size_t i = 0; i < size_; ++i){
          tmp[i] = data[i];
      }

      delete[] data;
      data = new T[cap];
      for(size_t i = 0; i < size_; ++i){
          data[i] = tmp[i];
      }
  }


  T* data{nullptr};
  T* end_{nullptr};
  size_t size_;
  size_t cap;
};
