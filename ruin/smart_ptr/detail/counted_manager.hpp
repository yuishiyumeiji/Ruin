//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RUIN_SMART_PTR_DETAIL_COUNTED_MANAGER_HPP_INCLUDED
#define RUIN_SMART_PTR_DETAIL_COUNTED_MANAGER_HPP_INCLUDED

#include <cstddef>
#include <typeinfo>
#include <utility>

namespace ruin
{
	namespace detail
	{
		class counted_manager_base
		{
		private:
			int use_count_;
		public:
			counted_manager_base()
				: use_count_(1)
			{ }
			virtual ~counted_manager_base()
			{ }
		public:
			virtual void destroy()
			{
				delete this;
			}
			virtual void dispose() = 0;
			virtual void* get_deleter(std::type_info const&) = 0;
			virtual void* get_untyped_deleter() = 0;
			void release()
			{
				if(--use_count_ == 0)
				{
					dispose();
					destroy();
				}
			}
			void add_ref()
			{
				++use_count_;
			}
			long use_count()
			{
				return use_count_;
			}
		};
		template<class P, class D>
		class counted_manager_impl_pd
			: public ruin::detail::counted_manager_base
		{
		private:
			P p_;
			D d_;
		public:
			counted_manager_impl_pd(P p, D d)
				: p_(p), d_(d)
			{ }
		public:
			virtual void dispose() override
			{
				d_(p_);
			}
			virtual void* get_deleter(std::type_info const& t) override
			{
				return t == typeid(D) ? &reinterpret_cast<char&>(d_) : nullptr;
			}
			virtual void* get_untyped_deleter() override
			{
				return &reinterpret_cast<char&>(d_);
			}
		};
		template<class P, class D, class A>
		class counted_manager_impl_pda
			: public ruin::detail::counted_manager_base
		{
		private:
			P p_;
			D d_;
			A a_;
		public:
			counted_manager_impl_pda(P p, D d, A a)
				: p_(p), d_(d), a_(a)
			{ }
		public:
			virtual void destroy() override
			{
				typedef typename A::template rebind<counted_manager_impl_pda>::other A2;
				A2 a2(a_);
				this->~counted_manager_impl_pda();
				a2.deallocate(this, 1);
			}
			virtual void dispose() override
			{
				d_(p_);
			}
			virtual void* get_deleter(std::type_info const& t) override
			{
				return t == typeid(D) ? &reinterpret_cast<char&>(d_) : nullptr;
			}
			virtual void* get_untyped_deleter() override
			{
				return &reinterpret_cast<char&>(d_);
			}
		};
		class counted_manager
		{
		private:
			ruin::detail::counted_manager_base* pi_;
		public:
			counted_manager()
				: pi_(nullptr)
			{ }
			template<class P, class D>
			explicit counted_manager(P p, D d)
			{
				try
				{
					pi_ = new ruin::detail::counted_manager_impl_pd<P, D>(p, d);
				}
				catch(...)
				{
					d(p), throw;
				}
			}
			template<class P, class D, class A>
			counted_manager(P p, D d, A a)
			{
				typedef ruin::detail::counted_manager_impl_pda<P, D, A> impl_type;
				typedef typename A::template rebind<impl_type>::other A2;
				A2 a2(a);
				try
				{
					pi_ = a2.allocate(1, static_cast<impl_type*>(nullptr));
					new(static_cast<void*>(pi_)) impl_type(p, d, a);
				}
				catch(...)
				{
					d(p);
					if(pi_)
					{
						a2.deallocate(static_cast<impl_type*>(pi_), 1);
					}
					throw;
				}
			}
			counted_manager(counted_manager const& r)
				: pi_(r.pi_)
			{
				if(pi_)
				{
					pi_->add_ref();
				}
			}
			counted_manager(counted_manager&& r)
				: pi_(r.pi_)
			{
				r.pi_ = nullptr;
			}
			~counted_manager()
			{
				if(pi_)
				{
					pi_->release();
				}
			}
		public:
			bool unique() const
			{
				return use_count() == 1;
			}
			long use_count() const
			{
				return pi_ ? pi_->use_count() : 0;
			}
			void* get_deleter(std::type_info const& t) const
			{
				return pi_ ? pi_->get_deleter(t) : nullptr;
			}
			void* get_untyped_deleter() const
			{
				return pi_ ? pi_->get_untyped_deleter() : nullptr;
			}
			void swap(counted_manager& other)
			{
				std::swap(pi_, other.pi_);
			}
		};
	}
}
	
#endif // RUIN_SMART_PTR_DETAIL_COUNTED_MANAGER_HPP_INCLUDED

