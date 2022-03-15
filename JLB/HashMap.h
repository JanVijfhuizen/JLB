#pragma once
#include "Array.h"
#include "KeyPair.h"

namespace jlb
{
	/// <summary>
	/// Data container that that prioritizes quick lookup speed.
	/// </summary>
	template <typename T>
	class HashMap : public Array<KeyPair<T>>
	{
	public:
		// Function used to get a hash value from a value.
		size_t(*hasher)(T& value);

		/// <summary>
		/// Inserts a value into the hashset. Does not store duplicates.
		/// </summary>
		/// <param name="value">Value to be inserted.</param>
		void Insert(T& value);
		/// <summary>
		/// Inserts a value into the hashset. Does not store duplicates.
		/// </summary>
		/// <param name="value">Value to be inserted.</param>
		void Insert(T&& value);
		/// <summary>
		/// Checks if the HashMap contains a certain value.
		/// </summary>
		/// <param name="value">Value to be checked.</param>
		/// <returns>If the HashMap contains the value.</returns>
		[[nodiscard]] bool Contains(T& value);
		/// <summary>
		/// Remove by value.
		/// </summary>
		/// <param name="value">Value to be removed.</param>
		void Erase(T& value);

		/// <summary>
		/// Gets the amount of values in the vector.
		/// </summary>
		/// <returns>Amount of values in the vector.</returns>
		[[nodiscard]] size_t GetCount() const;

	protected:
		[[nodiscard]] size_t GetHash(T& value);
		[[nodiscard]] bool Contains(T& value, size_t& outIndex);
		void Insert(size_t index, T& value);

		KeyPair<T>& operator[](size_t index) override;
		Iterator<KeyPair<T>> begin() override;
		Iterator<KeyPair<T>> end() override;

	private:
		size_t _count = 0;
	};

	template <typename T>
	void HashMap<T>::Insert(T& value)
	{
		Insert(GetHash(value), value);
	}

	template <typename T>
	void HashMap<T>::Insert(T&& value)
	{
		Insert(GetHash(value), value);
	}

	template <typename T>
	void HashMap<T>::Erase(T& value)
	{
		size_t index;
		const bool contains = Contains(value, index);
		assert(contains);

		const size_t length = Array<KeyPair<T>>::GetLength();
		assert(_count > 0);

		auto& keyPair = Array<KeyPair<T>>::operator[](index);

		// Check how big the key group is.
		size_t i = 1;
		while(i < length)
		{
			const size_t otherIndex = (index + i) % length;
			auto& otherKeyPair = Array<KeyPair<T>>::operator[](otherIndex);
			if (otherKeyPair.key != keyPair.key)
				break;
			++i;
		}

		// Setting the keypair value to the default value.
		keyPair = {};
		// Move the key group one place backwards by swapping the first and last index.
		Array<KeyPair<T>>::Swap(index, index + i - 1);
		--_count;
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
		assert(hasher);
		return hasher(value) % Array<KeyPair<T>>::GetLength();
	}

	template <typename T>
	bool HashMap<T>::Contains(T& value, size_t& outIndex)
	{
		const size_t length = Array<KeyPair<T>>::GetLength();
		assert(_count < length);

		int steps = 0;

		// Get and use the hash as an index.
		const size_t hash = GetHash(value);
		bool groupFound = false;
		for (size_t i = 0; i < length; ++i)
		{
			++steps;

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
	void HashMap<T>::Insert(const size_t index, T& value)
	{
		const size_t length = Array<KeyPair<T>>::GetLength();
		assert(_count < length);

		// If it already contains this value, replace the old one with the newer value.
		if (Contains(value))
			return;

		for (size_t i = 0; i < length; ++i)
		{
			const size_t modIndex = (index + i) % length;
			auto& keyPair = Array<KeyPair<T>>::operator[](modIndex);
			// Continue iterating until it finds a key with a higher value than the current hash.
			// If so, move them forward and place this one in it.
			if (keyPair.key <= index)
				continue;

			// See how many values need to step forward.
			size_t steps = 0;
			for (size_t j = i; j < length; ++j)
			{
				const size_t mod2Index = (modIndex + j) % length;
				auto& otherKeyPair = Array<KeyPair<T>>::operator[](mod2Index);
				if (otherKeyPair.key == SIZE_MAX)
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

			keyPair.key = index;
			keyPair.value = value;
			++_count;
			return;
		}

		// Should not reach this part of the code.
		assert(false);
	}

	template <typename T>
	KeyPair<T>& HashMap<T>::operator[](const size_t index)
	{
		return Array<KeyPair<T>>::operator[](index);
	}

	template <typename T>
	Iterator<KeyPair<T>> HashMap<T>::begin()
	{
		return Array<KeyPair<T>>::begin();
	}

	template <typename T>
	Iterator<KeyPair<T>> HashMap<T>::end()
	{
		return Array<KeyPair<T>>::end();
	}
}
