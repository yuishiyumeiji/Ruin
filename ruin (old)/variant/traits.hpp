
#ifndef RUIN_VARIANT_TRAITS_HPP_INCLUDED
#define RUIN_VARIANT_TRAITS_HPP_INCLUDED

namespace ruin { namespace variants
{
	template < class >
	struct variant_traits;

	template < class T >
	struct variant_traits<T const>
		: public ruin::variants::variant_traits<T>
	{ };

	template < class T >
	struct variant_traits<T volatile>
		: public ruin::variants::variant_traits<T>
	{ };

	template < class T >
	struct variant_traits<T const volatile>
		: public ruin::variants::variant_traits<T>
	{ };
}}

#endif // RUIN_VARIANT_TRAITS_HPP_INCLUDED

