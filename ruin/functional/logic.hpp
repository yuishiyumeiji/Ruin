//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_FUNCTIONAL_LOGIC_HPP_INCLUDED
#define RUIN_FUNCTIONAL_LOGIC_HPP_INCLUDED

namespace ruin
{
	struct land_t
	{
		template<class T, class U>
		constexpr auto operator()(T&& t, U&& u) const
		{
			return t && u;
		}
	};
	struct lor_t
	{
		template<class T, class U>
		constexpr auto operator()(T&& t, U&& u) const
		{
			return t || u;
		}
	};
	struct lnot_t
	{
		template<class T, class U>
		constexpr auto operator()(T&& t) const
		{
			return !t;
		}
	};
}

#endif // RUIN_FUNCTIONAL_LOGIC_HPP_INCLUDED

