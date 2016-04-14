
#ifndef RUIN_VARIANT_VISITOR_PTR_HPP_INCLUDED
#define RUIN_VARIANT_VISITOR_PTR_HPP_INCLUDED

#include <type_traits>
#include "ruin/mpl/if.hpp"
#include "ruin/utility/throw_and_return.hpp"
#include "ruin/variant/bad_visit.hpp"
#include "ruin/variant/static_visitor.hpp"

namespace ruin { namespace variants
{
	template < class T, class R >
	class visitor_ptr_t
		: public ruin::variants::static_visitor<R>
	{
	private:
		typedef R (*visitor_t)(T);
		typedef typename ruin::mpl::if_<std::is_reference<T>, T, T const&>::type argument_type;
	public:
		typedef R result_type;
	private:
		visitor_t visitor_;
	public:
		explicit constexpr
		visitor_ptr_t(visitor_t visitor)
			: visitor_(visitor)
		{ }
	public:
		constexpr result_type
		operator()(argument_type x) const
		{
			return visitor_(x);
		}
		template < class U >
		constexpr result_type
		operator()(U const&) const
		{
			return (throw ruin::bad_visit{}, ruin::throw_and_return<result_type>());
		}
	};

	template < class R, class T >
	constexpr ruin::variants::visitor_ptr_t<T, R>
	visitor_ptr(R (*visitor)(T))
	{
		return visitor_ptr_t<T, R>(visitor);
	}
}}

#endif // RUIN_VARIANT_VISITOR_PTR_HPP_INCLUDED

