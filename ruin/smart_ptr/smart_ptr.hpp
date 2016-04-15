//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RUIN_SMART_PTR_SMART_PTR_HPP_INCLUDED
#define RUIN_SMART_PTR_SMART_PTR_HPP_INCLUDED

#include <cstddef>
#include <memory>
#include <typeinfo>
#include <utility>
#include <ruin/smart_ptr/detail/traits.hpp>

namespace ruin
{
	template<class X, class Manager>
	class smart_ptr
	{
	public:
		typedef typename ruin::detail::sp_element<X>::type element_type;
		typedef element_type* pointer_type;
		typedef Manager manager_type;
	private:
		pointer_type p_;
		manager_type m_;
	public:
		smart_ptr()
			: p_(nullptr), m_()
		{ }
		smart_ptr(std::nullptr_t)
			: p_(nullptr), m_()
		{ }
		template<class Y>
		explicit smart_ptr(Y* p)
			: p_(p), m_(p, std::default_delete<Y>{})
		{ }
		template<class D>
		smart_ptr(std::nullptr_t p, D d)
			: p_(p), m_(p, d)
		{ }
		template<class Y, class D>
		smart_ptr(Y* p, D d)
			: p_(p), m_(p, d)
		{ }
		template<class D, class A>
		smart_ptr(std::nullptr_t p, D d, A a)
			: p_(p), m_(p, d, a)
		{ }
		template<class Y, class D, class A>
		smart_ptr(Y* p, D d, A a)
			: p_(p), m_(p, d, a)
		{ }
		smart_ptr(smart_ptr const& r)
			: p_(r.p_), m_(r.m_)
		{ }
		template<class Y>
		smart_ptr(smart_ptr<Y, Manager> const& r)
			: p_(r.p_), m_(r.m_)
		{ }
		template<class Y>
		smart_ptr(smart_ptr<Y, Manager> const& r, pointer_type p)
			: p_(p), m_(r.m_)
		{ }
		smart_ptr(smart_ptr&& r)
			: p_(r.p_), m_(std::move(r.m_))
		{
			r.p_ = nullptr;
		}
		template<class Y>
		smart_ptr(smart_ptr<Y, Manager>&& r)
			: p_(r.p_), m_(std::move(r.m_))
		{
			r.m_ = nullptr;
		}
		smart_ptr& operator=(std::nullptr_t)
		{
			smart_ptr().swap(*this);
			return *this;
		}
		smart_ptr& operator=(smart_ptr const& r)
		{
			smart_ptr(r).swap(*this);
			return *this;
		}
		template<class Y>
		smart_ptr& operator=(smart_ptr<Y, Manager> const& r)
		{
			smart_ptr(r).swap(*this);
			return *this;
		}
		smart_ptr& operator=(smart_ptr&& r)
		{
			smart_ptr(std::move(r)).swap(*this);
			return *this;
		}
		template<class Y>
		smart_ptr& operator=(smart_ptr<Y, Manager>&& r)
		{
			smart_ptr(std::move(r)).swap(*this);
			return *this;
		}
	public:
		void reset()
		{
			smart_ptr().swap(*this);
		}
		template<class Y>
		void reset(Y* p)
		{
			smart_ptr(p).swap(*this);
		}
		template<class Y, class D>
		void reset(Y* p, D d)
		{
			smart_ptr(p, d).swap(*this);
		}
		template<class Y, class D, class A>
		void reset(Y* p, D d, A a)
		{
			smart_ptr(p, d, a).swap(*this);
		}
		template<class Y>
		void reset(smart_ptr<Y, Manager> const& r, pointer_type p)
		{
			smart_ptr(r, p).swap(*this);
		}
		bool unique() const
		{
			return m_.unique();
		}
		long use_count() const
		{
			return m_.use_count();
		}
		typename ruin::detail::sp_dereference<X>::type operator*() const
		{
			return *p_;
		}
		typename ruin::detail::sp_member_access<X>::type operator->() const
		{
			return p_;
		}
		typename ruin::detail::sp_array_access<X>::type operator[](std::size_t i) const
		{
			return p_[i];
		}
		explicit operator bool() const
		{
			return p_ != nullptr;
		}
		bool operator!() const
		{
			return p_ == nullptr;
		}
		pointer_type get() const
		{
			return p_;
		}
		void* _internal_get_deleter(std::type_info const& t) const
		{
			return m_.get_deleter(t);
		}
		void* _internal_get_untyped_deleter() const
		{
			return m_.get_untyped_deleter();
		}
		void swap(smart_ptr& other)
		{
			std::swap(p_, other.p_);
			m_.swap(other.m_);
		}
	};
	
	template<class X, class Manager>
	inline bool operator==(smart_ptr<X, Manager> const& l, std::nullptr_t)
	{
		return l.get() == nullptr;
	}
	template<class Y, class Manager>
	inline bool operator==(std::nullptr_t, smart_ptr<Y, Manager> const& r)
	{
		return r.get() == nullptr;
	}
	template<class X, class Y, class Manager>
	inline bool operator==(smart_ptr<X, Manager> const& l, smart_ptr<Y, Manager> const& r)
	{
		return l.get() == r.get();
	}
	template<class X, class Manager>
	inline bool operator!=(smart_ptr<X, Manager> const& l, std::nullptr_t)
	{
		return l.get() != nullptr;
	}
	template<class Y, class Manager>
	inline bool operator!=(std::nullptr_t, smart_ptr<Y, Manager> const& r)
	{
		return r.get() != nullptr;
	}
	template<class X, class Y, class Manager>
	inline bool operator!=(smart_ptr<X, Manager> const& l, smart_ptr<Y, Manager> const& r)
	{
		return l.get() != r.get();
	}
	
	template<class Y, class X, class Manager>
	smart_ptr<Y, Manager> sp_static_cast(smart_ptr<X, Manager> const& r)
	{
		typedef typename smart_ptr<Y, Manager>::pointer_type P;
		P p = static_cast<P>(r.get());
		return smart_ptr<Y, Manager>(r, p);
	}
	template<class Y, class X, class Manager>
	smart_ptr<Y, Manager> sp_const_cast(smart_ptr<X, Manager> const& r)
	{
		typedef typename smart_ptr<Y, Manager>::pointer_type P;
		P p = const_cast<P>(r.get());
		return smart_ptr<Y, Manager>(r, p);
	}
	template<class Y, class X, class Manager>
	smart_ptr<Y, Manager> sp_dynamic_cast(smart_ptr<X, Manager> const& r)
	{
		typedef typename smart_ptr<Y, Manager>::pointer_type P;
		P p = dynamic_cast<P>(r.get());
		return p ? smart_ptr<Y, Manager>(r, p) : smart_ptr<Y, Manager>();
	}
	template<class Y, class X, class Manager>
	smart_ptr<Y, Manager> sp_reinterpret_cast(smart_ptr<X, Manager> const& r)
	{
		typedef typename smart_ptr<Y, Manager>::pointer_type P;
		P p = reinterpret_cast<P>(r.get());
		return smart_ptr<Y, Manager>(r, p);
	}
}

#endif // RUIN_SMART_PTR_SMART_PTR_HPP_INCLUDED

