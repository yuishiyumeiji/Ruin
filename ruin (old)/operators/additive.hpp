
#ifndef RUIN_OPERATORS_ADDITIVE_HPP_INCLUDED
#define RUIN_OPERATORS_ADDITIVE_HPP_INCLUDED

#include "ruin/operators/addable.hpp"
#include "ruin/operators/subtractable.hpp"

namespace ruin
{
	template < class T, class U = T >
	class additive
		: private ruin::addable<T, U>
		, private ruin::subtractable<T, U>
	{ };
}

#endif // RUIN_OPERATORS_ADDITIVE_HPP_INCLUDED
