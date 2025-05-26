#pragma once

#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <algorithm> 
#include "Iterator.h"

template <class T>
class Vector
{
private:
	T* data_;
	size_t size_;
	size_t capacity_;

	void reallocate(size_t new_capacity)
	{
		T* new_data = new T[new_capacity];

		if (data_ != nullptr)
		{
			try
			{
				std::copy(data_, data_ + size_, new_data);
			}
			catch (...)
			{
				delete[] new_data;
				throw;
			}
		}
		delete[] data_;
		data_ = new_data;
		capacity_ = new_capacity;
	}

public:
	Vector() : data_(nullptr), size_(0), capacity_(0) {}

	explicit Vector(size_t count, const T& value = T())
	{
		data_ = new T[count];
		size_ = count;
		capacity_ = count;
		std::fill(data_, data_ + size_, value);
	}

	Vector(std::initializer_list<T> init)
	{
		size_ = init.size();
		capacity_ = size_;
		data_ = new T[capacity_];
		std::copy(init.begin(), init.end(), data_);
	}

	~Vector()
	{
		delete[] data_;
		data_ = nullptr;
		size_ = 0;
		capacity_ = 0;
	}

	size_t size() const { return size_; }
	size_t capacity() const { return capacity_; }
	bool empty() { return size_ == 0; }

	void reserve(size_t new_capacity)
	{
		if (new_capacity > capacity_)
		{
			reallocate(new_capacity);
		}
	}

	void push_back(const T& value) 
	{
		if (size_ == capacity_)
		{
			size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
			reallocate(new_capacity);
		}
		data_[size_] = value;
		size_++;
	}

	void puch_back(T&& value) 
	{
		if (size_ == capacity_)
		{
			size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
			reallocate(new_capacity);
		}

		data_[size_] = std::move(value);
		size_++;
	}

	void pop_back()
	{
		if (size_ > 0)
		{
			size_--;
		}
	}

	void clear()
	{
		size_ = 0;
	}

	T& operator[](size_t index) { return data_[index]; }
	const T& operator[](size_t index) const { return data_[index]; }

	T& at(size_t index)
	{
		if (index >= size_)
		{
			throw std::out_of_range("Vector::at() index out of range");
		}
		return data_[index];
	}

	const T& at(size_t index) const
	{
		if (index >= size_)
		{
			throw std::out_of_range("Vector::at() index out of range");
		}
		return data_[index];
	}

	Vector(const Vector& other) : data_(nullptr), size_(0), capacity_(0)
	{
		if (other.capacity_ > 0)
		{
			data_ = new T[other.capacity_];
			capacity_ = other.capacity_;
			size_ = other.size_;

			try
			{
				std::copy(other.data_, other.data_ + other.size_, data_);
			}
			catch (...)
			{
				delete[] data_;
				data_ = nullptr;
				size_ = 0;
				capacity_ = 0;
				throw;
			}
		}
	}

	Vector& operator=(Vector other)
	{
		swap(*this, other);
		return *this;
	}

	Vector(Vector&& other) noexcept
		: data_(other.data_), size_(other.size_), capacity_(other.capacity_)
	{
		other.data_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}

	friend void swap(Vector& first, Vector& second)
	{
		using std::swap;
		swap(first.size_, second.size_);
		swap(first.capacity_, second.capacity_);
		swap(first.data_, second.data_);
	}

	T* begin() { return data_; }
	T* end() { return data_ + size_; }

	const T* begin() const { return data_; }
	const T* end() const { return data_ + size_; }
};
