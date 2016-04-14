//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_OPERATORS_DECREMENTABLE_HPP_INCLUDED
#define RUIN_OPERATORS_DECREMENTABLE_HPP_INCLUDED

namespace ruin
{
	template < class T >
	class decrementable
	{
		friend constexpr T operator--(T& lhs, int)
		{
			auto temp(lhs);
			--lhs;
			return temp;
		}
	};
}

#endif // RUIN_OPERATORS_DECREMENTABLE_HPP_INCLUDED
