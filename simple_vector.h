#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cstdlib>
#include <memory>
#include <utility>

template <typename T>
class SimpleVector {

private:
	int container_size;
	int container_capacity;
	T *container;

public:
    SimpleVector(int start_size = 1) {
		container_capacity = start_size;
		container = (T*)malloc(container_capacity * sizeof(T));
		container_size = 0;
	}

    SimpleVector(const SimpleVector &src) {
		container_size = container_capacity = src.container_size;
		container = (T*)malloc(container_size * sizeof(T));
		memcpy(container, src.container, container_size * sizeof(T));
	}

    T &operator[](int idx) const {
		return container[idx];
	}
	

    SimpleVector &operator=(const SimpleVector &src) {
		resize(src.container_size);
		memcpy(container, src.container, container_size * sizeof(T));
		return *this;
	}


    void assign(int size, const T& val) {
		resize(size);
		
		for (int i = 0; i < size; i++) {
			container[i] = val;
		}
	}

    void assign(T *begin, T *end) {
		int len = end - begin;
		resize(len);
		memcpy(container, begin, len * sizeof(T));
	}

    T &at(int idx) {
		if (idx >= container_size) {
			throw "out of range error";
		}
		return container[idx];
	}

    T &front() {
		return container[0];
	}

    T &back(){
		return container[container_size - 1];
	}

    void push_back(const T& val) {
		resize(container_size + 1);
		container[container_size - 1] = val;
	}

	void pop_back() {
		resize(container_size - 1);
	}

    void insert(T *pos, const T& val) {
		insert(pos, 1, val);
	}

    void insert(T *pos, int n, T val) {
		int countToMove = container + (container_size - pos);

		resize(container_size + n);
		
		for (int i = 0; i < countToMove; i++) {
			container[container_size - 1 - i] = container[container_size - 1 - i - n];
		}

		for (int i = 0; i < n; i++) {
			container[container_size - countToMove - i - 1] = val;
		}
	}

    void erase(T *pos) {
		erase(pos, pos + 1);
	}

    void erase(T *begin, T *end) {
		int len = end - begin;

		for (T *cur = end; cur <= end(); cur++) {
			*(cur - len) = *cur;
		}

		resize(container_size - len);
	}

    void clear() {
		resize(0);
	}

    size_t size() const {
		return container_size;
	}

    bool empty() const {
		return container_size == 0;
	}

    size_t capacity() const {
		return container_capacity;
	}

    void resize(int size) {
		if (size < container_size) {
			for (int i = size; i < container_size; i++) {
				(container + i)->~T();
			}
		}

		else if (size > container_capacity) {
			container_capacity = size * 2;
			T *temp = container;
			container = (T*)malloc(container_capacity * sizeof(T));
			memcpy(container, temp, container_size * sizeof(T));
			destroy(temp, container_size);
			free(temp);
		}
		container_size = size;
	}

	void destroy(T *ptr, int count) {
		while (count--) {
			(ptr++)->~T();
		}
	}

    T *begin() {
		return container;
	}

    T *end() {
		return container + container_size - 1;
	}

    void swap(SimpleVector &i) {
		const SimpleVector temp = *this;
		operator=(i);
		i = temp;
	}

    ~SimpleVector() {
		destroy(container, container_size);
		free(container);
	}
};

#endif