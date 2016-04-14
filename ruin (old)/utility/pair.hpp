
#ifndef RUIN_UTILITY_PAIR_HPP_INCLUDED
#define RUIN_UTILITY_PAIR_HPP_INCLUDED

#include <cstddef>
#include <type_traits>
#include "ruin/tuple/tuple_get.hpp"
#include "ruin/tuple/tuple_size.hpp"
#include "ruin/utility/index_tuple.hpp"
#include "ruin/utility/piecewise_construct.hpp"
#include "ruin/utility/forward.hpp"
#include "ruin/utility/swap.hpp"

namespace ruin {

template < class First, class Second >
struct pair
{
public:
	typedef First first_type;
	typedef Second second_type;

	First first_;
	Second second_;

	explicit constexpr
	pair (First const& first, Second const& second)
		: first_(first)
		, second_(second)
	{ }

private:
	template < class A1, std::size_t... Indexes1, class A2, std::size_t... Indexes2 >
	explicit constexpr
	pair (A1&& a1, ruin::index_tuple<Indexes1...>, A2&& a2, ruin::index_tuple<Indexes2...>)
		: first_(ruin::tuples::tuple_get<Indexes1>(ruin::forward<A1>(a1))...)
		, second_(ruin::tuples::tuple_get<Indexes2>(ruin::forward<A2>(a2))...)
	{ }

public:
	template < class A1, class A2 >
	explicit constexpr
	pair (ruin::piecewise_construct_t, A1&& a1, A2&& a2)
		: pair
			( ruin::forward<A1>(a1)
			, ruin::index_range<0, ruin::tuples::tuple_size<typename std::decay<A1>::type>::value>::make()
			, ruin::forward<A2>(a2)
			, ruin::index_range<0, ruin::tuples::tuple_size<typename std::decay<A2>::type>::value>::make()
			)
	{ }
	void
	swap (pair& y)
		noexcept(noexcept(ruin::swap(pair::first_, y.first_)) && noexcept(ruin::swap(pair::second_, y.second_)))
	{
		ruin::swap(first_, y.first_);
		ruin::swap(second_, y.second_);
	}
};

template < class First, class Second >
void
swap (ruin::pair<First, Second>& x, ruin::pair<First, Second>& y)
	noexcept(noexcept(x.swap(y)))
{
	x.swap(y);
}

}

#endif // RUIN_UTILITY_PAIR_HPP_INCLUDED
