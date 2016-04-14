//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_FUNCTIONAL_OVERLOADEDFUNCTION_HPP_INCLUDED
#define RUIN_FUNCTIONAL_OVERLOADEDFUNCTION_HPP_INCLUDED

#include <functional>
#include <tuple>
#include <utility>

namespace ruin
{
	namespace detain
	{
		template<class, int, class...>
		struct overloadedfunction_base;
		template<class Derived, int I, class R, class... Args, class... Functors>
		struct overloadedfunction_base<Derived, I, R (Args...), Functors...>
			: public ruin::functional::detail::overloadedfunction_base<Derived, I + 1, Functors...>
		{
			typedef ruin::functional::detail::overloadedfunction_base<Derived, I + 1, Functors...> inherited;
			constexpr R operator()(Args... args) const
			{
				return std::get<I>(static_cast<Derived const&>(*this).functors_)(std::forward<Args>(args)...);
			}
			template < class... As >
			constexpr auto operator()(As&&... as) const
				-> decltype(inherited{}(std::forward<As>(as)...))
			{
				return static_cast<inherited const&>(*this)(std::forward<As>(as)...);
			}
		};
		template<class Derived, int I>
		struct overloadedfunction_base<Derived, I>
		{ };
	}
	template<class... Functors>
	class overloadedfunction
		: public ruin::detail::overloadedfunction_base<ruin::overloadedfunction<Functors...>, 0, Functors...>
	{
	public:
		typedef ruin::detail::overloadedfunction_base<ruin::overloadedfunction<Functors...>, 0, Functors...> inherited;
		std::tuple<std::function<Functors>...> functors_;
	public:
		template<class... Fun>
		explicit constexpr overloadedfunction(Fun... fs)
			: functors_(fs...)
		{ }
	};
}

#endif // RUIN_FUNCTIONAL_OVERLOADEDFUNCTION_HPP_INCLUDED
