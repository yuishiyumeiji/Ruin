//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RUIN_SMART_PTR_DETAIL_TRAITS
#define RUIN_SMART_PTR_DETAIL_TRAITS

#include <cstddef>

namespace ruin
{
	namespace detail
	{
		template<class X>
		struct sp_element
		{
			typedef X type;
		};
		template<class X>
		struct sp_element<X[]>
		{
			typedef X type;
		};
		template<class X, std::size_t N>
		struct sp_element<X[N]>
		{
			typedef X type;
		};
		
		template<class X>
		struct sp_dereference
		{
			typedef X& type;
		};
		template<>
		struct sp_dereference<void>
		{
			typedef void type;
		};
		template<>
		struct sp_dereference<void const>
		{
			typedef void type;
		};
		template<>
		struct sp_dereference<void volatile>
		{
			typedef void type;
		};
		template<>
		struct sp_dereference<void const volatile>
		{
			typedef void type;
		};
		template<class X>
		struct sp_dereference<X[]>
		{
			typedef void type;
		};
		template<class X, std::size_t N>
		struct sp_dereference<X[N]>
		{
			typedef void type;
		};
		
		template<class X>
		struct sp_member_access
		{
			typedef X* type;
		};
		template<class X>
		struct sp_member_access<X[]>
		{
			typedef void type;
		};
		template<class X, std::size_t N>
		struct sp_member_access<X[N]>
		{
			typedef void type;
		};
		
		template<class X>
		struct sp_array_access
		{
			typedef void type;
		};
		template<class X>
		struct sp_array_access<X[]>
		{
			typedef X& type;
		};
		template<class X, std::size_t N>
		struct sp_array_access<X[N]>
		{
			typedef X& type;
		};
	}
}

#endif // RUIN_SMART_PTR_DETAIL_TRAITS

