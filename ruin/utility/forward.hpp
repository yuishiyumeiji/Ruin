//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_UTILITY_FORWARD_HPP_INCLUDED
#define RUIN_UTILITY_FORWARD_HPP_INCLUDED

#include <type_traits>

namespace ruin
{
	template<class T>
	constexpr T&& forward(T& t) noexcept
	{
		return static_cast<T&&>(t);
	}
	template<class T>
	void forward(typename std::remove_reference<T>::type&&) = delete;
}

#endif // RUIN_UTILITY_FORWARD_HPP_INCLUDED
