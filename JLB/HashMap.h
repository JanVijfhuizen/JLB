#pragma once
#include "Array.h"
#include "KeyPair.h"

namespace jlb
{
	template <typename T>
	class HashMap : public Array<KeyPair<T>>
	{
	public:
		size_t(*hasher)(T& value);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		T& Insert(T& value);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		T& Insert(T&& value);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		[[nodiscard]] bool Contains(T& value);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		void Erase(T& value);

		/// <summary>
		/// Gets the amount of values in the vector.
		/// </summary>
		/// <returns>Amount of values in the vector.</returns>
		[[nodiscard]] size_t GetCount() const;

		KeyPair<T>& operator[](size_t index) override = delete;

	protected:
		[[nodiscard]] size_t GetHash(T& value);
		[[nodiscard]] bool Contains(T& value, size_t& outIndex);
		[[nodiscard]] KeyPair<T>& Insert(size_t index, T& value);

	private:
		size_t _count = 0;
	};

	template <typename T>
	T& HashMap<T>::Insert(T& value)
	{
		return Insert(GetHash(value), value);
	}

	template <typename T>
	T& HashMap<T>::Insert(T&& value)
	{
		return Insert(GetHash(value), value);
	}

	template <typename T>
	void HashMap<T>::Erase(T& value)
	{
	}

	template <typename T>
	bool HashMap<T>::Contains(T& value)
	{
		size_t n;
		return Contains(value, n);
	}

	template <typename T>
	size_t HashMap<T>::GetCount() const
	{
		return _count;
	}

	template <typename T>
	size_t HashMap<T>::GetHash(T& value)
	{
		return hasher(value) % Array<KeyPair<T>>::GetLength();
	}

	template <typename T>
	bool HashMap<T>::Contains(T& value, size_t& outIndex)
	{
		const size_t length = Array<KeyPair<T>>::GetLength();
		assert(_count < length);

		// Get and use the hash as an index.
		const size_t hash = GetHash(value);
		bool groupFound = false;
		for (size_t i = 0; i < length; ++i)
		{
			const size_t index = (hash + i) % length;
			auto& keyPair = Array<KeyPair<T>>::operator[](index);
			// Set to true the first time the key group has been found.
			groupFound = groupFound ? true : keyPair.key == index;
			// Once this leaves the key group while it hasn't found the value, return nullptr.
			// OR if the keypair key is the default.
			if (groupFound && keyPair.key != hash || keyPair.key == SIZE_MAX)
				return false;
			// If this isn't in the key group just yet.
			if (!groupFound)
				continue;
			if (keyPair.value == value)
			{
				outIndex = index;
				return true;
			}
		}
		return false;
	}

	template <typename T>
	KeyPair<T>& HashMap<T>::Insert(const size_t index, T& value)
	{
		const size_t length = Array<KeyPair<T>>::GetLength();
		assert(_count < length);

		KeyPair<T>* keyPair = Contains(value);
		if (keyPair)
			return keyPair->value = value;

		for (size_t i = 0; i < length; ++i)
		{
			const size_t modIndex = (index + i) % length;
			auto& otherKeyPair = Array<KeyPair<T>>::operator[](modIndex);
			// Continue iterating until it finds a key with a higher value than the current hash.
			// If so, move them forward and place this one in it.
			if (otherKeyPair.key <= index)
				continue;

			// See how many values need to step forward.
			size_t steps = 0;
			for (size_t j = i; j < length; ++j)
			{
				const size_t mod2Index = (modIndex + j) % length;
				auto& other2KeyPair = Array<KeyPair<T>>::operator[](mod2Index);
				if (other2KeyPair.key == SIZE_MAX)
					break;
				++steps;
			}

			// Move all of them one step forward.
			for (size_t j = 0; j < steps; ++j)
			{
				const size_t fromIndex = (modIndex + steps - j - 1) % length;
				const size_t toIndex = (modIndex + steps - j) % length;

				auto& from = Array<KeyPair<T>>::operator[](fromIndex);
				auto& to = Array<KeyPair<T>>::operator[](toIndex);

				to = from;
			}

			otherKeyPair.key = index;
			otherKeyPair.value = value;
			return otherKeyPair;
		}

		// Should not reach this part of the code.
		assert(false);
		return  Array<KeyPair<T>>::operator[](0);
	}
}
