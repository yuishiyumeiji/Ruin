
#ifndef RUIN_TUPLE_ARRAY_HPP_INCLUDED
#define RUIN_TUPLE_ARRAY_HPP_INCLUDED

#include <cstddef>
#include "ruin/container/array.hpp"
#include "ruin/tuple/traits.hpp"
#include "ruin/utility/move.hpp"

namespace ruin { namespace tuples 
{
	template < class T, std::size_t N >
	struct tuple_traits<ruin::container::array<T, N>>
	{
		static constexpr std::size_t size = N;
		template < std::size_t >
		struct element
		{
			typedef T type;
		};
		template < std::size_t I >
		static T&
		get(ruin::container::array<T, N>& t)
		{
			return t[I];
		}
		template < std::size_t I >
		static constexpr T const&
		get(ruin::container::array<T, N> const& t)
		{
			return t[I];
		}
		template < std::size_t I >
		static constexpr T&&
		get(ruin::container::array<T, N>&& t)
		{
			return ruin::move(t[I]);
		}
	};	
}}

#endif // RUIN_TUPLE_ARRAY_HPP_INCLUDED
