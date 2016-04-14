
#ifndef RUIN_UTILITY_ENABLE_IF_HPP_INCLUDED
#define RUIN_UTILITY_ENABLE_IF_HPP_INCLUDED

namespace ruin
{
	template < bool Cond, class T = void >
	struct enable_if_c
	{
		typedef T type;
	};
	template < class T >
	struct enable_if_c<false, T>
	{ };

	template < class Cond, class T = void >
	using enable_if = ruin::enable_if_c<Cond::value, T>;

	template < bool Cond, class T >
	struct lazy_enable_if_c
	{
		typedef typename T::type type;
	};
	template < class T >
	struct lazy_enable_if_c<false, T>
	{ };

	template < class Cond, class T >
	using lazy_enable_if = ruin::lazy_enable_if_c<Cond::value, T>;

	template < bool Cond, class T = void >
	struct disable_if_c
	{
		typedef T type;
	};
	template < class T >
	struct disable_if_c<true, T>
	{ };

	template < class Cond, class T = void >
	using disable_if = ruin::disable_if_c<Cond::value, T>;

	template < bool Cond, class T >
	struct lazy_disable_if_c
	{
		typedef typename T::type type;
	};
	template < class T >
	struct lazy_disable_if_c<true, T>
	{
		typedef typename T::type type;
	};

	template < class Cond, class T >
	using lazy_disable_if = ruin::lazy_disable_if_c<Cond::value, T>;
}

#endif // RUIN_UTILITY_ENABLE_IF_HPP_INCLUDED

