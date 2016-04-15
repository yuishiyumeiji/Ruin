//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RUIN_SMART_PTR_DETAIL_LINKED_MANAGER_HPP_INCLUDED
#define RUIN_SMART_PTR_DETAIL_LINKED_MANAGER_HPP_INCLUDED

#include <cstddef>
#include <typeinfo>
#include <utility>

namespace ruin
{
	namespace detail
	{
		class linked_manager_base
		{
		public:
			virtual ~linked_manager_base()
			{ }
		public:
			virtual void destroy()
			{
				delete this;
			}
			virtual void dispose() = 0;
			virtual void* get_deleter(std::type_info const&) = 0;
			virtual void* get_untyped_deleter() = 0;
		};
		template<class P, class D>
		class linked_manager_impl_pd
			: public ruin::detail::linked_manager_base
		{
		private:
			P p_;
			D d_;
		public:
			linked_manager_impl_pd(P p, D d)
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
		class linked_manager_impl_pda
			: public ruin::detail::linked_manager_base
		{
		private:
			P p_;
			D d_;
			A a_;
		public:
			linked_manager_impl_pda(P p, D d, A a)
				: p_(p), d_(d), a_(a)
			{ }
		public:
			virtual void destroy() override
			{
				typedef typename A::template rebind<linked_manager_impl_pda>::other A2;
				A2 a2(a_);
				this->~linked_manager_impl_pda();
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
		class linked_manager
		{
		private:
			ruin::detail::linked_manager_base* pi_;
			mutable linked_manager const* prev_;
			mutable linked_manager const* next_;
		private:
			void insert(linked_manager const& other)
			{
				auto temp_ = next_;
				prev_->next_ = &other;
				temp_->prev_ = &other;
			}
		public:
			linked_manager()
				: pi_(nullptr)
			{
				prev_ = next_ = this;
			}
			template<class P, class D>
			linked_manager(P p, D d)
			{
				prev_ = next_ = this;
				try
				{
					pi_ = new ruin::detail::linked_manager_impl_pd<P, D>(p, d);
				}
				catch(...)
				{
					d(p), throw;
				}
			}
			template<class P, class D, class A>
			linked_manager(P p, D d, A a)
			{
				prev_ = next_ = this;
				typedef ruin::detail::linked_manager_impl_pda<P, D, A> impl_type;
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
			linked_manager(linked_manager const& r)
				: pi_(r.pi_), prev_(&r), next_(r.next_)
			{
				insert(*this);
			}
			linked_manager(linked_manager&& r)
				: pi_(r.pi_)
			{
				r.insert(*this);
				prev_ = r.prev_;
				next_ = r.next_;
				r.prev_ = &r;
				r.next_ = &r;
				r.pi_ = nullptr;
			}
			~linked_manager()
			{
				if(unique())
				{
					pi_->dispose();
					pi_->destroy();
				}
				else
				{
					prev_->next_ = next_;
					next_->prev_ = prev_;
				}
			}
		public:
			bool unique() const
			{
				return pi_ && next_ == this;
			}
			long use_count() const
			{
				if(!pi_)
				{
					return 0;
				}
				long i = 1;
				linked_manager const* p = next_;
				while(p != this)
				{
					p = p->next_;
					++i;
				}
				return i;
			}
			void* get_deleter(std::type_info const& t) const
			{
				return pi_ ? pi_->get_deleter(t) : nullptr;
			}
			void* get_untyped_deleter() const
			{
				return pi_ ? pi_->get_untyped_deleter() : nullptr;
			}
			void swap(linked_manager& other)
			{
				if(pi_ != other.pi_)
				{
					insert(other);
					other.insert(*this);
					std::swap(pi_, other.pi_);
					std::swap(prev_, other.prev_);
					std::swap(next_, other.next_);
				}
			}
		};
	}
}
	
#endif // RUIN_SMART_PTR_DETAIL_LINKED_MANAGER_HPP_INCLUDED

