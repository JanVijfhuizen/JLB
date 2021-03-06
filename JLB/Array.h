#pragma once
#include <cassert>
#include "LinearAllocator.h"
#include "Iterator.h"
#include <cstring>

namespace jlb
{
	/// <summary>
	/// Array that does not have ownership over the memory that it uses.
	/// </summary>
	template <typename T>
	class Array
	{
	public:
		Array() = default;
		Array(Array& other) = delete;
		Array(Array&& other) = delete;
		Array& operator=(Array& other) = delete;
		Array& operator=(Array&& other) = delete;
		virtual ~Array() = default;

		[[nodiscard]] virtual T& operator[](size_t index);
		[[nodiscard]] size_t GetLength() const;

		/// <summary>
		/// Allocates a chunk of memory to be managed.<br>
		/// The view does not own this memory, and does not free any memory (previously) managed.
		/// </summary>
		/// <param name="allocator">Allocator from which to allocate.</param>
		/// <param name="size">Size of the array.</param>
		/// <param name="fillValue">The array will be initialized with this value.</param>
		virtual void Allocate(LinearAllocator& allocator, size_t size, const T& fillValue = {});

		/// <summary>
		/// Allocates a chunk of memory to be managed.<br>
		/// The view does not own this memory, and does not free any memory (previously) managed.
		/// </summary>
		/// <param name="allocator">Allocator from which to allocate.</param>
		/// <param name="size">Size of the array.</param>
		/// <param name="src">The data to copy into the array.</param>
		virtual void Allocate(LinearAllocator& allocator, size_t size, T* src);

		/// <summary>
		/// Frees the array from the linear allocator.
		/// </summary>
		/// <param name="allocator">Allocator to free it from.</param>
		virtual void Free(LinearAllocator& allocator);

		/// <summary>
		/// Swaps values at the defined indexes.
		/// </summary>
		/// <param name="a">Index A.</param>
		/// <param name="b">Index B.</param>
		void Swap(size_t a, size_t b);

		/// <summary>
		/// Get a raw pointer to the managed memory.
		/// </summary>
		/// <returns>Raw pointer to the managed memory.</returns>
		[[nodiscard]] T* GetData();

		[[nodiscard]] virtual Iterator<T> begin();
		[[nodiscard]] virtual Iterator<T> end();

	private:
		T* _memory = nullptr;
		size_t _length = 0;
	};

	template <typename T>
	T& Array<T>::operator[](const size_t index)
	{
		assert(index < _length);
		return _memory[index];
	}

	template <typename T>
	size_t Array<T>::GetLength() const
	{
		return _length;
	}

	template <typename T>
	void Array<T>::Allocate(LinearAllocator& allocator, const size_t size, const T& fillValue)
	{
		_memory = allocator.New<T>(size);
		_length = size;

		for (size_t i = 0; i < size; ++i)
			_memory[i] = fillValue;
	}

	template <typename T>
	void Array<T>::Allocate(LinearAllocator& allocator, const size_t size, T* src)
	{
		_memory = allocator.New<T>(size);
		_length = size;

		memcpy(_memory, src, size * sizeof(T));
	}

	template <typename T>
	void Array<T>::Free(LinearAllocator& allocator)
	{
		allocator.Free();
	}

	template <typename T>
	void Array<T>::Swap(const size_t a, const size_t b)
	{
		assert(a < _length&& b < _length);
		const T temp = _memory[a];
		_memory[a] = _memory[b];
		_memory[b] = temp;
	}

	template <typename T>
	T* Array<T>::GetData()
	{
		return _memory;
	}

	template <typename T>
	Iterator<T> Array<T>::begin()
	{
		Iterator<T> it;
		it.memory = _memory;
		it.index = 0;
		it.length = _length;
		return it;
	}

	template <typename T>
	Iterator<T> Array<T>::end()
	{
		Iterator<T> it;
		it.memory = _memory;
		it.index = _length;
		it.length = _length;
		return it;
	}
}