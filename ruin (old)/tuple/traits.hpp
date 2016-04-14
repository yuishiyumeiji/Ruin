
#ifndef RUIN_TUPLE_TRAITS_HPP_INCLUDED
#define RUIN_TUPLE_TRAITS_HPP_INCLUDED

namespace ruin { namespace tuples
{
	template < class >
	struct tuple_traits;

	template < class T >
	struct tuple_traits<T const>
		: public ruin::tuples::tuple_traits<T>
	{ };
	template < class T >
	struct tuple_traits<T volatile>
		: public ruin::tuples::tuple_traits<T>
	{ };
	template < class T >
	struct tuple_traits<T const volatile>
		: public ruin::tuples::tuple_traits<T>
	{ };
}}

#endif // RUIN_TUPLE_TRAITS_HPP_INCLUDED
