
#ifndef RUIN_OPERATORS_RIGHT_SHIFTABLE_HPP_INCLUDED
#define RUIN_OPERATORS_RIGHT_SHIFTABLE_HPP_INCLUDED

#include "ruin/utility/move.hpp"

namespace ruin
{
	template < class T, class U = T >
	class right_shiftable
	{
		friend T&
		operator>>=(T& lhs, U const& rhs)
		{
			return lhs = ruin::move(lhs) >> rhs;
		}
		friend T&
		operator>>=(T& lhs, U&& rhs)
		{
			return lhs = ruin::move(lhs) >> ruin::move(rhs);
		}
	};
}

#endif // RUIN_OPERATORS_RIGHT_SHIFTABLE_HPP_INCLUDED
