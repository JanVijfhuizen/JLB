#include "UnitTest.h"
#include "LinearAllocator.h"
#include <cstdlib>
#include <cassert>
#include "ArrayView.h"
#include "VectorView.h"

void UnitTest::Run()
{
	// Test linear allocator malloc/free.
	for (size_t i = 0; i < 25; ++i)
	{
		const size_t size = 512 + rand() % 512;
		LinearAllocator allocator{size};
		allocator.Malloc(64 + rand() % 128);
		const size_t remainingStart = allocator.GetAvailableMemorySpace();

		for (size_t j = 0; j < 25; ++j)
		{
			const size_t remaining = allocator.GetAvailableMemorySpace();
			const size_t depth = 2 + rand() % 2;

			for (size_t k = 0; k < depth; ++k)
				allocator.Malloc(rand() % 128);

			for (size_t k = 0; k < depth; ++k)
				allocator.Free();

			assert(remaining == allocator.GetAvailableMemorySpace());
		}

		assert(remainingStart == allocator.GetAvailableMemorySpace());
		allocator.Free();
	}

	// Test linear allocator new/delete.
	for (size_t i = 0; i < 25; ++i)
	{
		const size_t size = 2056 + rand() % 512;
		LinearAllocator allocator{ size };

		struct TestStruct final
		{
			float f;
			int i;
			bool b;
		};

		allocator.New<TestStruct>(1 + rand() % 10);
		const size_t remainingStart = allocator.GetAvailableMemorySpace();

		for (size_t j = 0; j < 25; ++j)
		{
			const size_t remaining = allocator.GetAvailableMemorySpace();
			const size_t depth = 2 + rand() % 2;

			for (size_t k = 0; k < depth; ++k)
				allocator.New<TestStruct>(rand() % 16);

			for (size_t k = 0; k < depth; ++k)
				allocator.Free();

			assert(remaining == allocator.GetAvailableMemorySpace());
		}

		assert(remainingStart == allocator.GetAvailableMemorySpace());
		allocator.Free();
	}

	// Test array view.
	{
		struct TestStruct final
		{
			float f;
			int i;
			bool b;
		} str{};

		str.i = 14;
		str.b = true;

		for (size_t i = 0; i < 25; ++i)
		{
			LinearAllocator allocator{ 1024 };
			ArrayView<TestStruct> array{};

			array.Allocate(allocator, 5 + rand() % 20, str);
			for (size_t j = 0; j < array.GetLength(); ++j)
			{
				assert(array[j].i == str.i);
				assert(array[j].b == str.b);
			}
		}

		LinearAllocator allocator{ 1024 };
		ArrayView<TestStruct> array{};
		array.Allocate(allocator, 12 + rand() % 12, str);

		for (auto& test : array)
		{
			assert(test.i == str.i);
			assert(test.b == str.b);
		}

		for (int i = 0; i < 25; ++i)
		{
			ArrayView<TestStruct> a{};
			a.Allocate(allocator, 6 + rand() % 12);

			ArrayView<TestStruct> b{};
			b.Allocate(allocator, a.GetLength(), a.GetData());

			allocator.Free();
			allocator.Free();
		}

		ArrayView<int> arr{};
		arr.Allocate(allocator, 4);
		arr[1] = 4;
		arr[2] = 6;
		arr.Swap(1, 2);
		assert(arr[1] == 6 && arr[2] == 4);
	}

	{
		LinearAllocator allocator{ 1024 };
		VectorView<int> vec{};
		vec.Allocate(allocator, 12);
		vec.SetCount(2);
		vec[0] = 2;
		vec[1] = 4;
		vec.Add(6);
		vec[3] = 8;

		int n = 0;
		for (auto& i : vec)
		{
			n++;
			assert(n <= 3);
			assert(i != 8);
			assert(n * 2 == i);
		}
	}
}
