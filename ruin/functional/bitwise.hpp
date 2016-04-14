//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_FUNCTIONAL_BITWISE_HPP_INCLUDED
#define RUIN_FUNCTIONAL_BITWISE_HPP_INCLUDED

namespace ruin
{
	struct and_t
	{
		template<class T, class U>
		constexpr auto operator()(T&& t, U&& u) const
			-> decltype(t & u)
		{
			return t & u;
		}
	};
	struct or_t
	{
		template<class T, class U>
		constexpr auto operator()(T&& t, U&& u) const
			-> decltype(t | u)
		{
			return t | u;
		}
	};
	struct xor_t
	{
		template<class T, class U>
		constexpr auto operator()(T&& t, U&& u) const
			-> decltype(t ^ u)
		{
			return t ^ u;
		}
	};
	struct not_t
	{
		template<class T, class U>
		constexpr auto operator()(T&& t) const
			-> decltype(~t)
		{
			return ~t;
		}
	};
}

#endif // RUIN_FUNCTIONAL_BITWISE_HPP_INCLUDED

