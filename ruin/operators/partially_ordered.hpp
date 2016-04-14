//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_OPERATORS_PARTIALLY_ORDERED_HPP_INCLUDED
#define RUIN_OPERATORS_PARTIALLY_ORDERED_HPP_INCLUDED

namespace ruin
{
	template < class T, class U = T >
	class partially_ordered
	{
		friend constexpr bool operator<=(T const& lhs, U const& rhs)
		{
			return lhs < rhs || lhs == rhs;
		}
		friend constexpr bool operator>=(T const& lhs, U const& rhs)
		{
			return lhs > rhs || lhs == rhs;
		}
		friend constexpr bool operator<(U const& lhs, T const& rhs)
		{
			return rhs > lhs;
		}
		friend constexpr bool operator>(U const& lhs, T const& rhs)
		{
			return rhs < lhs;
		}
		friend constexpr bool operator<=(U const& lhs, T const& rhs)
		{
			return rhs >= lhs;
		}
		friend constexpr bool operator>=(U const& lhs, T const& rhs)
		{
			return rhs <= lhs;
		}
	};
	template < class T >
	class partially_ordered<T, T>
	{
		friend constexpr bool operator>(T const& lhs, T const& rhs)
		{
			return rhs < lhs;
		}
		friend constexpr bool operator<=(T const& lhs, T const& rhs)
		{
			return lhs < rhs || lhs == rhs;
		}
		friend constexpr bool operator>=(T const& lhs, T const& rhs)
		{
			return rhs < lhs || lhs == rhs;
		}
	};
}

#endif // RUIN_OPERATORS_PARTIALLY_ORDERED_HPP_INCLUDED
