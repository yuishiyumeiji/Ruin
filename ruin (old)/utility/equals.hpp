
#ifndef RUIN_UTILITY_EQUALS_HPP_INCLUDED
#define RUIN_UTILITY_EQUALS_HPP_INCLUDED

#include "ruin/utility/forward.hpp"

namespace ruin
{
	struct equals_t
	{
		template < class L, class R >
		constexpr auto
		operator()(L&& lhs, R&& rhs) const
			noexcept(noexcept(ruin::forward<L>(lhs) == ruin::forward<R>(rhs)))
			-> decltype(ruin::forward<L>(lhs) == ruin::forward<R>(rhs))
		{
			return ruin::forward<L>(lhs) == ruin::forward<R>(rhs);
		}
		template < class L, class R >
		constexpr bool
		operator()(L* lhs, R* rhs) const noexcept
		{
			return reinterpret_cast<void const volatile*>(lhs) == reinterpret_cast<void const volatile*>(rhs);
		}
	};
	static constexpr ruin::equals_t equals{};
}

#endif // RUIN_UTILITY_EQUALS_HPP_INCLUDED
