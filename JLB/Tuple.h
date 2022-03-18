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
		public:
			/// <summary>
			/// Checks if this tuple contains a certain type.
			/// </summary>
			/// <typeparam name="T">The type to check.</typeparam>
			/// <returns>If the tuple contains the type.</returns>
			template <typename T>
			[[nodiscard]] static bool ContainsType()
			{
				return _ContainsType<T, Head, Tail...>();
			}

		private:
			template <typename T, typename U, typename ...Args>
			static bool _ContainsType()
			{
				if (typeid(T) == typeid(U))
					return true;
				if constexpr (sizeof...(Args) > 0)
					return _ContainsType<T, Args...>();
				return false;
			}
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
