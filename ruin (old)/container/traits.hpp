
#ifndef RUIN_CONTAINER_TRAITS_HPP_INCLUDED
#define RUIN_CONTAINER_TRAITS_HPP_INCLUDED

#include <cstddef>

namespace ruin { namespace container
{
	namespace detail
	{
		template < class Container >
		struct traits
		{
			typedef typename Container::value_type value_type;
			typedef typename Container::reference reference;
			typedef typename Container::const_reference const_reference;
			typedef typename Container::pointer pointer;
			typedef typename Container::const_pointer const_pointer;
			typedef typename Container::size_type size_type;
			typedef typename Container::difference_type difference_type;
		};
		template < class T, std::size_t N >
		struct traits<T[N]>
		{
			typedef T value_type;
			typedef T& reference;
			typedef T const& const_reference;
			typedef T* pointer;
			typedef T const* const_pointer;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
		};
	} // namespace detail

	template < class Container >
	struct container_traits
		: public ruin::container::detail::traits<Container>
	{ };
	template < class Container >
	struct container_traits<Container const>
		: public ruin::container::detail::traits<Container>
	{
		typedef typename ruin::container::detail::traits<Container>::const_pointer pointer;
		typedef typename ruin::container::detail::traits<Container>::const_reference reference;
	};
}}

#endif // RUIN_CONTAINER_TRAITS_HPP_INCLUDED
