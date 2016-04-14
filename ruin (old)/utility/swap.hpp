
#ifndef RUIN_UTILITY_SWAP_HPP_INCLUDED
#define RUIN_UTILITY_SWAP_HPP_INCLUDED

#include <cstddef>
#include <utility>

namespace ruin_swap_impl
{
	// If argument dependent lookup fails, then use std::swap
	using namespace std;

	template < class T1, class T2 >
	void
	swap_impl (T1& x, T2& y)
		noexcept(noexcept(swap(x, y)))
	{
		swap(x, y);
	}
}

namespace ruin {

template < class T1, class T2 >
void
swap (T1& x, T2& y)
	noexcept(noexcept(ruin_swap_impl::swap_impl(x, y)))
{
	ruin_swap_impl::swap_impl(x, y);
}
template < class T1, class T2, std::size_t N >
void
swap (T1 (& x)[N], T2 (& y)[N])
	noexcept(noexcept(ruin::swap(x[0], y[0])))
{
	for (std::size_t i = 0 ; i < N ; ++i)
	{
		ruin::swap(x[i], y[i]);
	}
}

}

#endif // RUIN_UTILITY_SWAP_HPP_INCLUDED
