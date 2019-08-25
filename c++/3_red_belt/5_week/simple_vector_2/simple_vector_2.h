#include <cstdint>
#include <utility>
#include <iterator>
using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
  SimpleVector() = default;
  explicit SimpleVector(size_t size);
  ~SimpleVector();

  T& operator[](size_t index);

  T* begin();
  T* end();

  size_t Size() const;
  size_t Capacity() const;
  void PushBack(T value);

  // При необходимости перегрузите
  // существующие публичные методы

private:
  T* data{nullptr};
  size_t size = 0;
  size_t capacity = 0;
};

template <typename T>
SimpleVector<T>::SimpleVector(size_t size_) {
  size = size_;
  data = new T[size];
  capacity = size;
}

template <typename T>
SimpleVector<T>::~SimpleVector(){
  delete[] data;
}

template<typename T>
T& SimpleVector<T>::operator[](size_t index){
  return data[index];
}

template<typename T>
T* SimpleVector<T>::begin() {
  return data;
}

template<typename T>
T* SimpleVector<T>::end() {
  return data + size;
}

template<typename T>
size_t SimpleVector<T>::Size() const {
  return size;
}

template<typename T>
size_t SimpleVector<T>::Capacity() const {
  return capacity;
}

template<typename T>
void SimpleVector<T>::PushBack(T value) {
  if (size >= capacity){
    capacity = capacity == 0 ? 1 : capacity * 2;
    auto new_data = new T[capacity];
    move(begin(), end(), new_data);
    delete[] data;
    data = new_data;
  }
  data[size++] = move(value);
}