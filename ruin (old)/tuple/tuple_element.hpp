
#ifndef RUIN_TUPLE_TUPLE_ELEMENT_HPP_INCLUDED
#define RUIN_TUPLE_TUPLE_ELEMENT_HPP_INCLUDED

#include <cstddef>
#include "ruin/tuple/traits.hpp"

namespace ruin { namespace tuples
{
	template < class T, std::size_t I >
	struct tuple_element
		: public ruin::tuples::tuple_traits<T>::template element<I>
	{ };
}}

#endif // RUIN_TUPLE_TUPLE_ELEMENT_HPP_INCLUDED
