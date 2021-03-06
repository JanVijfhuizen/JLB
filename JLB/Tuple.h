#pragma once

namespace jlb
{
	namespace tupleImpl
	{
		template <size_t I, typename T>
		struct TupleLeaf
		{
			T value;
		};

		template <size_t I, typename ...Ts>
		struct TupleImpl;

		template <size_t I>
		struct TupleImpl<I>
		{

		};

		template <size_t I, typename Head, typename ...Tail>
		struct TupleImpl<I, Head, Tail...> : TupleLeaf<I, Head>, TupleImpl<I + 1, Tail...>
		{
			
		};
	}

	/// <summary>
	/// Gets the value in the tuple at index I.
	/// </summary>
	/// <returns>Value at index I.</returns>
	template<size_t I, typename Head, typename ...Tail>
	[[nodiscard]] Head& Get(tupleImpl::TupleImpl<I, Head, Tail...>& tuple)
	{
		return tuple.TupleLeaf<I, Head>::value;
	}

	/// <summary>
	/// Struct that can hold multiple different types.
	/// </summary>
	/// <typeparam name="...Ts">Types to be held.</typeparam>
	template <typename ...Ts>
	using Tuple = tupleImpl::TupleImpl<0, Ts...>;
}
