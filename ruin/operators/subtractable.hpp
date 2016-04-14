//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_OPERATORS_SUBTRACTABLE_HPP_INCLUDED
#define RUIN_OPERATORS_SUBTRACTABLE_HPP_INCLUDED

#include "ruin/utility/move.hpp"

namespace ruin
{
	template < class T, class U = T >
	class subtractable
	{
		friend T&
		operator-=(T& lhs, U const& rhs)
		{
			return lhs = ruin::move(lhs) - rhs;
		}
		friend T&
		operator-=(T& lhs, U&& rhs)
		{
			return lhs = ruin::move(lhs) - ruin::move(rhs);
		}
	};
}

#endif // RUIN_OPERATORS_SUBTRACTABLE_HPP_INCLUDED
