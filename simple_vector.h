/*
* simple_vector.h
*
* A simple implementation of a vector, based on the latest C++ container spec (draft)
* which can be found at http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3797.pdf
* 
* This container is not 100% standards compliant, is missing some features, and may not be as
* fast as std::vector but is functional enough for our program's needs.
*
*/

#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <iostream>
#include <cstddef>
#include <stdexcept>

#define RESIZE_MULTIPLIER 1.5

template<class T> class SimpleVector {
  public:

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T value_type;
    typedef size_t size_type;

    T& operator[] (size_type idx);
    const T& operator[] (size_type idx) const;
    SimpleVector& operator= (SimpleVector vec) noexcept;

    T& at(size_type idx);
    const T& at(size_type idx) const;
    void clear();
    bool empty() const;
    size_type size() const;
    void push_back(const T& item);
    void pop_back();
    void resize(size_type sz);
    void resize(size_type sz, const value_type& obj);
    void reserve(size_type sz);

    iterator begin();
    iterator end();
    iterator back();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator back() const;

    SimpleVector(size_type sz, const value_type& obj = T());
    SimpleVector(const SimpleVector& vec);
    SimpleVector(SimpleVector&& vec);

    SimpleVector();
    ~SimpleVector();

    friend void swap(SimpleVector& first, SimpleVector& second) {
      std::swap(first.container_size, second.container_size);
      std::swap(first.container_capacity, second.container_capacity);
      std::swap(first.data_chunk, second.data_chunk);
    }

  private:
    size_type container_size;
    size_type container_capacity;
    std::allocator<T> mem_allocator;
    T* data_chunk;

    void make_container_smaller(size_type sz);
    void make_container_bigger(size_type sz);
};

template<class T>
SimpleVector<T>::SimpleVector(size_type sz, const value_type& obj) {
  container_size = sz;
  container_capacity = sz;
  data_chunk = mem_allocator.allocate(container_capacity);
  for (iterator iter = data_chunk; iter != data_chunk + container_size; ++iter) {
    mem_allocator.construct(iter, obj);
  }
}

template<class T>
SimpleVector<T>::SimpleVector(const SimpleVector& vec) {
  container_size = vec.container_size;
  container_capacity = vec.container_capacity;
  data_chunk = mem_allocator.allocate(container_capacity);
  for (size_type i = 0; i < container_size; i++) {
    mem_allocator.construct(data_chunk + i, vec.data_chunk[i]);
  }
}

template<class T>
SimpleVector<T>::SimpleVector(SimpleVector&& vec) : SimpleVector()
{
  swap(*this, vec);
}

template<class T>
SimpleVector<T>::SimpleVector() {
  container_capacity = 0;
  container_size = 0;
  data_chunk = nullptr;
}

template<class T>
SimpleVector<T>::~SimpleVector() {
  for (iterator iter = data_chunk; iter != data_chunk + container_size; ++iter) {
    mem_allocator.destroy(iter);
  }
  mem_allocator.deallocate(data_chunk,container_capacity);
}

template<class T>
T& SimpleVector<T>::operator[] (size_type idx) {
  return data_chunk[idx];
}

template<class T>
const T& SimpleVector<T>::operator[] (size_type idx) const {
  return data_chunk[idx];
}

template<class T>
SimpleVector<T>& SimpleVector<T>::operator=(SimpleVector<T> vec) noexcept {
  swap(*this, vec);
  return *this;
}

template<class T>
T& SimpleVector<T>::at(size_type idx) {
  if (idx >= container_size) throw std::out_of_range("Index out of range");
  return data_chunk[idx];
}

template<class T>
const T& SimpleVector<T>::at(size_type idx) const {
  if (idx >= container_size) throw std::out_of_range("Index out of range");
  return data_chunk[idx];
}

template<class T>
void SimpleVector<T>::clear() {
  for (iterator iter = data_chunk; iter != data_chunk + container_size; ++iter) {
    mem_allocator.destroy(iter);
  }
  mem_allocator.deallocate(data_chunk, container_capacity);
}

template<class T>
bool SimpleVector<T>::empty() const {
  return container_size == 0;
}

template<class T>
typename SimpleVector<T>::size_type SimpleVector<T>::size() const {
  return container_size;
}

template<class T>
void SimpleVector<T>::push_back(const T& item) {
  resize(container_size + 1, item);
}

template<class T>
void SimpleVector<T>::pop_back() {
  resize(container_size - 1);
}

template<class T>
void SimpleVector<T>::resize(size_type sz) {
  if (container_size == sz) return;

  if (sz < container_size) {
    make_container_smaller(sz);
    return;
  }

  if (sz > container_capacity) {
    make_container_bigger(sz);
  }

  for (iterator it = data_chunk + container_size; it != data_chunk + sz; ++it) {
    mem_allocator.construct(it);
  }

  container_size = sz;
}

template<class T>
void SimpleVector<T>::resize(size_type sz, const value_type& obj) {
  if (container_size == sz) return;
  if (sz < container_size) {
    make_container_smaller(sz);
    return;
  }
  if (sz > container_capacity) {
    make_container_bigger(sz);
  }

  for (iterator it = data_chunk + container_size; it != data_chunk + sz; ++it) {
    mem_allocator.construct(it, obj);
  }
  container_size = sz;
}

template<class T>
void SimpleVector<T>::reserve(size_type sz) {
  if (sz <= container_capacity) {
    return;
  }

  T* prev_data = data_chunk;
  size_type prev_capacity = container_capacity;

  data_chunk = mem_allocator.allocate(sz);
  container_capacity = sz;

  for (size_type i = 0; i < container_size; i++) {
    mem_allocator.construct(data_chunk + i, prev_data[i]);
  }

  for (size_type i=0; i<container_size; i++) {
    mem_allocator.destroy(prev_data + i);
  }

  mem_allocator.deallocate(prev_data, prev_capacity);
}

template<class T>
void SimpleVector<T>::make_container_bigger(size_type sz) {
  size_type new_capacity;
  if (container_capacity == 0) {
    new_capacity = 1;
  } else if (container_capacity < std::numeric_limits<size_type>::max() / 2) {
    new_capacity = container_capacity * RESIZE_MULTIPLIER;
  } else {
    new_capacity = std::numeric_limits<size_type>::max();
  }

  if (sz > new_capacity) new_capacity = sz;

  reserve(new_capacity);
}

template<class T>
void SimpleVector<T>::make_container_smaller(size_type sz) {
  for (iterator iter = data_chunk + sz; iter != data_chunk + container_size; ++iter) {
    mem_allocator.destroy(iter);
  }
  container_size = sz;
}

template<class T>
typename SimpleVector<T>::iterator SimpleVector<T>::back() {
  return &data_chunk[container_size - 1];
}

template<class T>
typename SimpleVector<T>::const_iterator SimpleVector<T>::back() const {
  return &data_chunk[container_size - 1];
}

template<class T>
typename SimpleVector<T>::iterator SimpleVector<T>::begin() {
  return &data_chunk[0];
}

template<class T>
typename SimpleVector<T>::const_iterator SimpleVector<T>::begin() const {
  return &data_chunk[0];
}

template<class T>
typename SimpleVector<T>::iterator SimpleVector<T>::end() {
  return &data_chunk[container_size];
}

template<class T>
typename SimpleVector<T>::const_iterator SimpleVector<T>::end() const {
  return &data_chunk[container_size];
}

#endif