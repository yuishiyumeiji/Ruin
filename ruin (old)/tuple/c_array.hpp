
#ifndef RUIN_TUPLE_C_ARRAY_HPP_INCLUDED
#define RUIN_TUPLE_C_ARRAY_HPP_INCLUDED

#include <cstddef>
#include "ruin/tuple/traits.hpp"
#include "ruin/utility/move.hpp"

namespace ruin { namespace tuples 
{
	template < class T, std::size_t N >
	struct tuple_traits<T[N]>
	{
		static constexpr std::size_t size = N;
		template < std::size_t >
		struct element
		{
			typedef T type;
		};
		template < std::size_t I >
		static T&
		get(T (& t)[N])
		{
			static_assert(I < N, "get<>: index out of range");
			return t[I];
		}
		template < std::size_t I >
		static constexpr T const&
		get(T const (& t)[N])
		{
			static_assert(I < N, "get<>: index out of range");
			return t[I];
		}
		template < std::size_t I >
		static constexpr T&&
		get(T (&& t)[N])
		{
			static_assert(I < N, "get<>: index out of range");
			return ruin::move(t[I]);
		}
	};	
}}

#endif // RUIN_TUPLE_C_ARRAY_HPP_INCLUDED
