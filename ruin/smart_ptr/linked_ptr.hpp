//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RUIN_SMART_PTR_LINKED_PTR_HPP_INCLUDED
#define RUIN_SMART_PTR_LINKED_PTR_HPP_INCLUDED

#include <ruin/smart_ptr/smart_ptr.hpp>
#include <ruin/smart_ptr/detail/linked_manager.hpp>

namespace ruin
{
	template<class X>
	using linked_ptr = ruin::smart_ptr<X, ruin::detail::linked_manager>;
}

#endif // RUIN_SMART_PTR_LINKED_PTR_HPP_INCLUDED

