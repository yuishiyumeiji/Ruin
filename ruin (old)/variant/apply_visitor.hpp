
#ifndef RUIN_VARIANT_APPLY_VISITOR_HPP_INCLUDED
#define RUIN_VARIANT_APPLY_VISITOR_HPP_INCLUDED

#include <type_traits>
#include "ruin/utility/forward.hpp"
#include "ruin/variant/traits.hpp"

namespace ruin { namespace variants
{
	template < class Visitor, class Visitable, class Traits = ruin::variants::variant_traits<typename std::decay<Visitable>::type> >
	constexpr auto
	apply_visitor(Visitor&& visitor, Visitable&& visitable)
		-> decltype(Traits::apply_visitor(ruin::forward<Visitor>(visitor), ruin::forward<Visitable>(visitable)))
	{
		return Traits::apply_visitor(ruin::forward<Visitor>(visitor), ruin::forward<Visitable>(visitable));
	}
}}

#endif // RUIN_VARIANT_APPLY_VISITOR_HPP_INCLUDED

