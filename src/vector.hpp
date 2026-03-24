#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		/**
		 * add data members
		 *   just add whatever you want.
		 */
		T* current;
		const vector<T>* container;

	public:
		iterator(T* ptr = nullptr, const vector<T>* cont = nullptr)
			: current(ptr), container(cont) {}

		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			return iterator(current + n, container);
		}
		iterator operator-(const int &n) const
		{
			return iterator(current - n, container);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (container != rhs.container) {
				throw invalid_iterator();
			}
			return current - rhs.current;
		}
		iterator& operator+=(const int &n)
		{
			current += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			current -= n;
			return *this;
		}
		/**
		 * iter++
		 */
		iterator operator++(int)
		{
			iterator tmp = *this;
			current++;
			return tmp;
		}
		/**
		 * ++iter
		 */
		iterator& operator++()
		{
			current++;
			return *this;
		}
		/**
		 * iter--
		 */
		iterator operator--(int)
		{
			iterator tmp = *this;
			current--;
			return tmp;
		}
		/**
		 * --iter
		 */
		iterator& operator--()
		{
			current--;
			return *this;
		}
		/**
		 * *it
		 */
		T& operator*() const
		{
			return *current;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const
		{
			return current == rhs.current && container == rhs.container;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return current == rhs.current && container == rhs.container;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return !(*this == rhs);
		}

		T* operator->() const
		{
			return current;
		}

		friend class vector;
	};
	/**
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		const T* current;
		const vector<T>* container;

	public:
		const_iterator(const T* ptr = nullptr, const vector<T>* cont = nullptr)
			: current(ptr), container(cont) {}

		const_iterator(const iterator& other)
			: current(other.current), container(other.container) {}

		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			return const_iterator(current + n, container);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(current - n, container);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			if (container != rhs.container) {
				throw invalid_iterator();
			}
			return current - rhs.current;
		}
		const_iterator& operator+=(const int &n)
		{
			current += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			current -= n;
			return *this;
		}
		/**
		 * iter++
		 */
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			current++;
			return tmp;
		}
		/**
		 * ++iter
		 */
		const_iterator& operator++()
		{
			current++;
			return *this;
		}
		/**
		 * iter--
		 */
		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			current--;
			return tmp;
		}
		/**
		 * --iter
		 */
		const_iterator& operator--()
		{
			current--;
			return *this;
		}
		/**
		 * *it
		 */
		const T& operator*() const
		{
			return *current;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const const_iterator &rhs) const
		{
			return current == rhs.current && container == rhs.container;
		}
		bool operator==(const iterator &rhs) const
		{
			return current == rhs.current && container == rhs.container;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const const_iterator &rhs) const
		{
			return !(*this == rhs);
		}
		bool operator!=(const iterator &rhs) const
		{
			return !(*this == rhs);
		}

		const T* operator->() const
		{
			return current;
		}

		friend class vector;
	};

private:
	T* data;
	size_t current_size;
	size_t capacity;

	void expand_capacity(size_t new_capacity) {
		if (new_capacity <= capacity) return;

		char* new_data = new char[new_capacity * sizeof(T)];
		for (size_t i = 0; i < current_size; ++i) {
			new (&new_data[i * sizeof(T)]) T(data[i]);
			data[i].~T();
		}
		delete[] (char*)data;
		data = (T*)new_data;
		capacity = new_capacity;
	}

	void construct_range(size_t start, size_t end, const T& value) {
		for (size_t i = start; i < end; ++i) {
			new (&data[i]) T(value);
		}
	}

	void destroy_range(size_t start, size_t end) {
		for (size_t i = start; i < end; ++i) {
			data[i].~T();
		}
	}

public:
	/**
	 * Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data(nullptr), current_size(0), capacity(0) {}

	vector(const vector &other) : data(nullptr), current_size(0), capacity(0) {
		if (other.current_size > 0) {
			data = (T*)new char[other.current_size * sizeof(T)];
			capacity = other.current_size;
			current_size = other.current_size;

			for (size_t i = 0; i < current_size; ++i) {
				new (&data[i]) T(other.data[i]);
			}
		}
	}

	/**
	 * Destructor
	 */
	~vector() {
		clear();
		if (data) {
			delete[] (char*)data;
		}
	}

	/**
	 * Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == &other) return *this;

		clear();
		if (data) {
			delete[] (char*)data;
			data = nullptr;
		}

		current_size = 0;
		capacity = 0;

		if (other.current_size > 0) {
			data = (T*)new char[other.current_size * sizeof(T)];
			capacity = other.current_size;
			current_size = other.current_size;

			for (size_t i = 0; i < current_size; ++i) {
				new (&data[i]) T(other.data[i]);
			}
		}

		return *this;
	}

	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}

	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (current_size == 0) {
			throw container_is_empty();
		}
		return data[0];
	}

	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (current_size == 0) {
			throw container_is_empty();
		}
		return data[current_size - 1];
	}

	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data, this);
	}
	const_iterator begin() const {
		return const_iterator(data, this);
	}
	const_iterator cbegin() const {
		return const_iterator(data, this);
	}

	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data + current_size, this);
	}
	const_iterator end() const {
		return const_iterator(data + current_size, this);
	}
	const_iterator cend() const {
		return const_iterator(data + current_size, this);
	}

	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return current_size == 0;
	}

	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return current_size;
	}

	/**
	 * clears the contents
	 */
	void clear() {
		destroy_range(0, current_size);
		current_size = 0;
	}

	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		if (pos.container != this) {
			throw invalid_iterator();
		}

		size_t index = pos.current - data;
		return insert(index, value);
	}

	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > current_size) {
			throw index_out_of_bound();
		}

		if (current_size >= capacity) {
			size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
			expand_capacity(new_capacity);
		}

		// Move elements after ind to the right
		for (size_t i = current_size; i > ind; --i) {
			new (&data[i]) T(data[i - 1]);
			data[i - 1].~T();
		}

		// Insert the new element
		new (&data[ind]) T(value);
		current_size++;

		return iterator(data + ind, this);
	}

	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		if (pos.container != this) {
			throw invalid_iterator();
		}

		size_t index = pos.current - data;
		return erase(index);
	}

	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= current_size) {
			throw index_out_of_bound();
		}

		// Destroy the element at ind
		data[ind].~T();

		// Move elements after ind to the left
		for (size_t i = ind; i < current_size - 1; ++i) {
			new (&data[i]) T(data[i + 1]);
			data[i + 1].~T();
		}

		current_size--;

		// Return iterator to the element that followed the erased one
		return iterator(data + ind, this);
	}

	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (current_size >= capacity) {
			size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
			expand_capacity(new_capacity);
		}

		new (&data[current_size]) T(value);
		current_size++;
	}

	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (current_size == 0) {
			throw container_is_empty();
		}

		data[current_size - 1].~T();
		current_size--;
	}
};


}

#endif
