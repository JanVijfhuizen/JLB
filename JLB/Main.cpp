#include "LinearAllocator.h"
#include <cassert>

int main()
{
	LinearAllocator allocator{264};

	size_t remaining = allocator.GetAvailableMemorySpace();
	allocator.Malloc(144);
	size_t r2 = allocator.GetAvailableMemorySpace();
	allocator.Malloc(54);
	allocator.Free();
	allocator.Malloc(32);
	allocator.Free();

	assert(r2 == allocator.GetAvailableMemorySpace());

	return 0;
}
