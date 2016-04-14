
#ifndef RUIN_OPERATORS_MULTIPLICATIVE_HPP_INCLUDED
#define RUIN_OPERATORS_MULTIPLICATIVE_HPP_INCLUDED

#include "ruin/operators/dividable.hpp"
#include "ruin/operators/multipliable.hpp"

namespace ruin
{
	template < class T, class U = T >
	class multiplicative
		: private ruin::multipliable<T, U>
		, private ruin::dividable<T, U>
	{ };
}

#endif // RUIN_OPERATORS_MULTIPLICATIVE_HPP_INCLUDED
