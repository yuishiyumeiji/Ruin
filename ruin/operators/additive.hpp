//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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
