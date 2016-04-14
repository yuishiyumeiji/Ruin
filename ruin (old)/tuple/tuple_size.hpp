
#ifndef RUIN_TUPLE_TUPLE_SIZE_HPP_INCLUDED
#define RUIN_TUPLE_TUPLE_SIZE_HPP_INCLUDED

#include <type_traits>
#include "ruin/tuple/traits.hpp"

namespace ruin { namespace tuples
{
	template < class T >
	struct tuple_size
		: public std::integral_constant<std::size_t, ruin::tuples::tuple_traits<T>::size>
	{ };
}}

#endif // RUIN_TUPLE_TUPLE_SIZE_HPP_INCLUDED
