#pragma once
#include <cassert>
#include "LinearAllocator.h"

/// <summary>
/// Array that does not have ownership over the memory that it uses.
/// </summary>
template <typename T>
class ArrayView
{
public:
	[[nodiscard]] T& operator[](size_t index);
	[[nodiscard]] size_t GetLength() const;

	/// <summary>
	/// Allocates a chunk of memory to be managed.<br>
	/// The view does not own this memory, and does not free any memory (previously) managed.
	/// </summary>
	/// <param name="allocator">Allocator from which to allocate.</param>
	/// <param name="size">Size of the array.</param>
	/// <param name="fillValue">The array will be initialized with this value.</param>
	void Allocate(LinearAllocator& allocator, size_t size, const T& fillValue = {});

private:
	T* _memory = nullptr;
	size_t _length = 0;
};

template <typename T>
T& ArrayView<T>::operator[](const size_t index)
{
	assert(index < _length);
	return _memory[index];
}

template <typename T>
size_t ArrayView<T>::GetLength() const
{
	return _length;
}

template <typename T>
void ArrayView<T>::Allocate(LinearAllocator& allocator, const size_t size, const T& fillValue)
{
	_memory = allocator.New<T>(size);
	_length = size;

	for (size_t i = 0; i < size; ++i)
		_memory[i] = fillValue;
}
