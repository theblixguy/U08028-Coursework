/*
* simple_vector.h
*
* Written by: Suyash Srijan
* Student number: 14076594
*
* A simple implementation of a vector, based on the latest C++ container spec (draft)
* which can be found at http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3797.pdf
* 
* This container is not 100% standards compliant, is missing some features, and may not be as
* optimized as std::vector but is functional enough for our program's needs.
*
*/

#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

/* Imports we need */
#include <iostream>
#include <cstddef>
#include <stdexcept>

/* Memory resize multiplier 
*
* Controls the growth rate of the vector's capacity
*
* 1.5 provides a good balance between speed and space while still making sure push_back's are amortized O(1). 
* Some implementations use 2 but it's up to whether you want to prioritize performance or memory usage. Having 
* a much lower resize multiplier (say 1.2) would actually result in substantial decrease in performance (due 
* to constant reallocations) and having a much higher resize multiplier (say 3) would result in substantial 
* increase in memory usage and caching issues while providing little to no performance improvements.
* 
*/
#define RESIZE_MULTIPLIER 1.5

/* SimpleVector */
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
    value_type* data() noexcept;
    const value_type* data() const noexcept;
    bool empty() const;
    size_type size() const;
    void push_back(const T& item);
    void pop_back();
    void resize(size_type sz);
    void resize(size_type sz, const value_type& obj);
    void reserve(size_type sz);

    iterator begin() noexcept;
    iterator end() noexcept;
    iterator back();
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
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

/* Constructor */
template<class T>
SimpleVector<T>::SimpleVector(size_type sz, const value_type& obj) {
  container_size = sz;
  container_capacity = sz;
  data_chunk = mem_allocator.allocate(container_capacity);
  for (iterator iter = data_chunk; iter != data_chunk + container_size; ++iter) {
    mem_allocator.construct(iter, obj);
  }
}

/* Constructor */
template<class T>
SimpleVector<T>::SimpleVector(const SimpleVector& vec) {
  container_size = vec.container_size;
  container_capacity = vec.container_capacity;
  data_chunk = mem_allocator.allocate(container_capacity);
  for (size_type i = 0; i < container_size; i++) {
    mem_allocator.construct(data_chunk + i, vec.data_chunk[i]);
  }
}

/* Constructor */
template<class T>
SimpleVector<T>::SimpleVector(SimpleVector&& vec) : SimpleVector() {
  swap(*this, vec);
}

/* Constructor */
template<class T>
SimpleVector<T>::SimpleVector() {
  container_capacity = 0;
  container_size = 0;
  data_chunk = nullptr;
}

/* Destructor
*
* Iterate though all the elements in our storage chunk and call their destructors, and then finally deallocate the chunk
*/
template<class T>
SimpleVector<T>::~SimpleVector() {
  for (iterator iter = data_chunk; iter != data_chunk + container_size; ++iter) {
    mem_allocator.destroy(iter);
  }
  mem_allocator.deallocate(data_chunk, container_capacity);
}

/* Return the element stored at idx or throw out_of_range exception if the element doesn't exist */
template<class T>
T& SimpleVector<T>::operator[] (size_type idx) {
  if (idx >= container_size) throw std::out_of_range("Index out of range");
  return data_chunk[idx];
}

/* Return the element stored at idx or throw out_of_range exception if the element doesn't exist */
template<class T>
const T& SimpleVector<T>::operator[] (size_type idx) const {
  if (idx >= container_size) throw std::out_of_range("Index out of range");
  return data_chunk[idx];
}

/* Swap vectors on move */
template<class T>
SimpleVector<T>& SimpleVector<T>::operator=(SimpleVector<T> vec) noexcept {
  swap(*this, vec);
  return *this;
}

/* Return the element stored at idx or throw out_of_range exception if the element doesn't exist */
template<class T>
T& SimpleVector<T>::at(size_type idx) {
  if (idx >= container_size) throw std::out_of_range("Index out of range");
  return data_chunk[idx];
}

/* Return the element stored at idx or throw out_of_range exception if the element doesn't exist */
template<class T>
const T& SimpleVector<T>::at(size_type idx) const {
  if (idx >= container_size) throw std::out_of_range("Index out of range");
  return data_chunk[idx];
}

/* Basically the same as ~SimpleVector() */
template<class T>
void SimpleVector<T>::clear() {
  for (iterator iter = data_chunk; iter != data_chunk + container_size; ++iter) {
    mem_allocator.destroy(iter);
  }
  mem_allocator.deallocate(data_chunk, container_capacity);
}

/* Returns a pointer to the first element in the container */
template<class T>
typename SimpleVector<T>::value_type* SimpleVector<T>::data() noexcept {
  return begin();
}

/* Returns a pointer to the first element in the container */
template<class T>
const typename SimpleVector<T>::value_type* SimpleVector<T>::data() const noexcept {
  return begin();
}

/* Check if the container has any elements and return the result */
template<class T>
bool SimpleVector<T>::empty() const {
  return container_size == 0;
}

/* Return the number of elements in the container */
template<class T>
typename SimpleVector<T>::size_type SimpleVector<T>::size() const {
  return container_size;
}

/* Insert an element into the container */
template<class T>
void SimpleVector<T>::push_back(const T& item) {
  resize(container_size + 1, item);
}

/* Remove the last item from the container */
template<class T>
void SimpleVector<T>::pop_back() {
  resize(container_size - 1);
}

/* Resize the storage
*
*  If the size of the container is bigger than the passed size then allocate more storage to
*  the container, otherwise reduce the amount of storage allocated to the container
*/
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

/* Same as above except you can pass obj to the copy constructor's argument */ 
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

/* Reserve storage for the container
* 
*  Allocate more storage to the container but leave it uninitialized. Copy data from the
*  old data_chunk to the new one
*/
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

  for (size_type i=0; i < container_size; i++) {
    mem_allocator.destroy(prev_data + i);
  }

  mem_allocator.deallocate(prev_data, prev_capacity);
}

/* Increase the container's storage capacity by the resize multiplier */
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

/* Decrease the container's storage capacity */
template<class T>
void SimpleVector<T>::make_container_smaller(size_type sz) {
  for (iterator iter = data_chunk + sz; iter != data_chunk + container_size; ++iter) {
    mem_allocator.destroy(iter);
  }
  container_size = sz;
}

/* Return an iterator to the last element in the container */
template<class T>
typename SimpleVector<T>::iterator SimpleVector<T>::back() {
  return &data_chunk[container_size - 1];
}

/* Return a const iterator to the last element in the container */
template<class T>
typename SimpleVector<T>::const_iterator SimpleVector<T>::back() const {
  return &data_chunk[container_size - 1];
}

/* Return an iterator to the first element in the container */
template<class T>
typename SimpleVector<T>::iterator SimpleVector<T>::begin() noexcept {
  return &data_chunk[0];
}

/* Return a const iterator to the first element in the container */
template<class T>
typename SimpleVector<T>::const_iterator SimpleVector<T>::begin() const noexcept {
  return &data_chunk[0];
}

/* Return an iterator to the past-the-end element in the container */
template<class T>
typename SimpleVector<T>::iterator SimpleVector<T>::end() noexcept {
  return &data_chunk[container_size];
}

/* Return a const iterator to the past-the-end element in the container */
template<class T>
typename SimpleVector<T>::const_iterator SimpleVector<T>::end() const noexcept {
  return &data_chunk[container_size];
}

#endif