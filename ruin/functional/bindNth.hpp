//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_FUNCTIONAL_BINDNTH_HPP_INCLUDED
#define RUIN_FUNCTIONAL_BINDNTH_HPP_INCLUDED

#include <cstddef>
#include <tuple>
#include <utility>
#include "ruin/utility/index_tuple.hpp"

namespace ruin
{
	namespace detail
	{
		template<std::size_t N>
		struct bindNth_impl
		{
			template<class F, class X, std::size_t... Indices>
			constexpr auto operator()(F&& f, X&& x, ruin::index_tuple<Indices...>) const
				-> decltype(f(std::get<Indices == N ? 0 : Indices < N ? Indices + 1 : Indices>(std::forward<X>(x))...))
			{
				return f(std::get<Indices == N ? 0 : Indices < N ? Indices + 1 : Indices>(std::forward<X>(x))...);
			}
		};
		template < std::size_t N, class F, class X >
		struct binderNth
		{
			F f_;
			X x_;
			template <class... As>
			constexpr auto operator()(As&&... as) const
				-> decltype(ruin::detail::bindNth_impl<N>{}(binderNth::f_, std::forward_as_tuple(binderNth::x_, std::forward<As>(as)...), ruin::index_range<0, sizeof...(As) + 1>::make()))
			{
				return ruin::detail::bindNth_impl<N>{}(f_, std::forward_as_tuple(x_, std::forward<As>(as)...), ruin::index_range<0, sizeof...(As) + 1>::make());
			}
		};
	}
	template<std::size_t N, class F, class X>
	constexpr ruin::detail::binderNth<N, F, X> bindNth(F&& f, X&& x)
	{
		return {std::forward<F>(f), std::forward<X>(x)};
	}
}

#endif // RUIN_FUNCTIONA_BINDNTH_HPP_INCLUDED

