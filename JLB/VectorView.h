#pragma once
#include "ArrayView.h"

/// <summary>
/// Unordered vector that does not have ownership over the memory that it uses.<br>
/// It does not resize the capacity automatically.
/// </summary>
template <typename T>
class VectorView : public ArrayView<T>
{
public:
	/// <summary>
	/// Place a value in the front of the vector and increase it's size by one.<br>
	/// Cannot exceed the capacity of the managed memory.
	/// </summary>
	/// <param name="value">The value to be added to the vector.</param>
	/// <returns>The added value inside the vector.</returns>
	T& Add(T&& value = {});
	/// <summary>
	/// Remove the value at a certain index.
	/// </summary>
	/// <param name="index">Index where the value will be removed.</param>
	void RemoveAt(size_t index);
	/// <summary>
	/// Set the count of the vector. Cannot exceed the capacity of the managed memory.
	/// </summary>
	/// <param name="count"></param>
	void SetCount(size_t count);
	/// <summary>
	/// Gets the amount of values in the vector.
	/// </summary>
	/// <returns>Amount of values in the vector.</returns>
	[[nodiscard]] size_t GetCount() const;
	[[nodiscard]] Iterator<T> end() override;

private:
	// The amount of values in this vector.
	size_t _count = 0;
};

template <typename T>
T& VectorView<T>::Add(T&& value)
{
	assert(_count + 1 <= ArrayView<T>::GetLength());
	return ArrayView<T>::operator[](_count++) = value;
}

template <typename T>
void VectorView<T>::RemoveAt(const size_t index)
{
	// Swap the removed value with the last instance.
	ArrayView<T>::Swap(index, --_count);
}

template <typename T>
void VectorView<T>::SetCount(const size_t count)
{
	assert(count <= ArrayView<T>::GetLength());
	_count = count;
}

template <typename T>
size_t VectorView<T>::GetCount() const
{
	return _count;
}

template <typename T>
Iterator<T> VectorView<T>::end()
{
	Iterator<T> it;
	it.memory = ArrayView<T>::GetData();
	it.index = _count;
	it.length = ArrayView<T>::GetLength();
	return it;
}
