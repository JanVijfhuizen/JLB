#pragma once
#include <cstdint>

namespace jlb
{
	template <typename T>
	struct KeyPair final
	{
		T value{};
		size_t key = SIZE_MAX;
	};
}
