//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RUIN_SMART_PTR_MAKE_SMART_HPP_INCLUDED
#define RUIN_SMART_PTR_MAKE_SMART_HPP_INCLUDED

#include <type_traits>
#include <utility>
#include "ruin/smart_ptr/smart_ptr.hpp"

namespace ruin
{
	namespace detail
	{
		template<class T>
		class sp_ms_deleter
		{
		private:
			typedef std::aligned_storage<sizeof(T), alignof(T)> storage_type;
		private:
			storage_type storage_;
			bool initialized_;
		private:
			void destroy()
			{
				if(initialized_)
				{
					reinterpret_cast<T*>(address())->~T();
					initialized_ = false;
				}
			}
		public:
			sp_ms_deleter()
				: initialized_(false)
			{ }
			sp_ms_deleter(sp_ms_deleter const&)
				: initialized_(false)
			{ }
			~sp_ms_deleter()
			{
				destroy();
			}
			void operator()(T*)
			{
				destroy();
			}
			void* address()
			{
				return &storage_;
			}
			void set_initialized()
			{
				initialized_ = true;
			}
		};
	}

	template<class T, class Policy, class... As>
	ruin::smart_ptr<T, Policy> make_smart(As&&... as)
	{
		ruin::smart_ptr<T, Policy> p(static_cast<T*>(nullptr), ruin::detail::sp_ms_deleter<T>{});
		ruin::detail::sp_ms_deleter<T>* pd = static_cast<ruin::detail::sp_ms_deleter<T>*>(p._internal_get_untyped_deleter());
		void* pst = pd->address();
		::new(pst) T(std::forward<As>(as)...);
		pd->set_initialized();
		T* px = static_cast<T*>(pst);
		return ruin::smart_ptr<T, Policy>(std::move(p), px);
	}
	template<class T, class Policy, class A, class... As>
	ruin::smart_ptr<T, Policy> allocate_smart(A const& a, As&... as)
	{
		ruin::smart_ptr<T, Policy> p(static_cast<T*>(nullptr), ruin::detail::sp_ms_deleter<T>{}, a);
		ruin::detail::sp_ms_deleter<T>* pd = static_cast<ruin::detail::sp_ms_deleter<T>*>(p._internal_get_untyped_deleter());
		void* pst = pd->address();
		::new(pst) T(std::forward<As>(as)...);
		pd->set_initialized();
		T* px = static_cast<T*>(pst);
		return ruin::smart_ptr<T, Policy>(std::move(p), px);
	}
}

#endif // RUIN_SMART_PTR_MAKE_SMART_HPP_INCLUDED

