
#ifndef RUIN_ITERATOR_REVERSE_ITERATOR_HPP_INCLUDED
#define RUIN_ITERATOR_REVERSE_ITERATOR_HPP_INCLUDED

#include <iterator>
#include "ruin/iterator/traits.hpp"
#include "ruin/operators/additive.hpp"
#include "ruin/operators/decrementable.hpp"
#include "ruin/operators/equality_comparable.hpp"
#include "ruin/operators/incrementable.hpp"
#include "ruin/operators/less_than_comparable.hpp"
#include "ruin/utility/address_of.hpp"
#include "ruin/utility/swap.hpp"

namespace ruin
{
	namespace iterator
	{
		template < class Iter >
		class reverse_iterator
			: public std::iterator
				< typename ruin::iterator::iterator_traits<Iter>::iterator_category
				, typename ruin::iterator::iterator_traits<Iter>::value_type
				, typename ruin::iterator::iterator_traits<Iter>::difference_type
				, typename ruin::iterator::iterator_traits<Iter>::pointer
				, typename ruin::iterator::iterator_traits<Iter>::reference
				>
			, private ruin::additive<ruin::iterator::reverse_iterator<Iter>, typename ruin::iterator::iterator_traits<Iter>::difference_type>
			, private ruin::incrementable<ruin::iterator::reverse_iterator<Iter>>
			, private ruin::decrementable<ruin::iterator::reverse_iterator<Iter>>
			, private ruin::equality_comparable<ruin::iterator::reverse_iterator<Iter>>
			, private ruin::less_than_comparable<ruin::iterator::reverse_iterator<Iter>>
		{
			template < class >
			friend class ruin::iterator::reverse_iterator;
		private:
			typedef ruin::iterator::iterator_traits<Iter> traits_type;
		public:
			typedef Iter iterator_type;
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type value_type;
			typedef typename traits_type::difference_type difference_type;
			typedef typename traits_type::reference reference;
			typedef typename traits_type::pointer pointer;
		protected:
			Iter p_;
		private:
			Iter deref_;
		public:
			reverse_iterator () = default;
			reverse_iterator (reverse_iterator const&) = default;
			explicit constexpr
			reverse_iterator (iterator_type p)
				: p_(p)
				, deref_(p-1)
			{ }
		public:
			void
			swap (reverse_iterator& y)
				noexcept(noexcept(ruin::swap(reverse_iterator::p_, y.p_)))
			{
				ruin::swap(p_, y.p_);
				ruin::swap(deref_, y.deref_);
			}
			constexpr reverse_iterator
			next () const
			{
				return reverse_iterator(p_ - 1);
			}
			constexpr reverse_iterator
			prev () const
			{
				return reverse_iterator(p_ + 1);
			}
			reverse_iterator&
			operator= (reverse_iterator const& r)
			{
				reverse_iterator(r).swap(*this);
				return *this;
			}
			constexpr reference
			operator* () const
			{
				return *deref_;
			}
			constexpr pointer
			operator-> () const
			{
				return ruin::address_of(*deref_);
			}
			constexpr reference
			operator[] (difference_type i) const
			{
				return *(deref_ - i);
			}
			reverse_iterator&
			operator++ ()
			{
				next().swap(*this);
				return *this;
			}
			reverse_iterator&
			operator-- ()
			{
				prev().swap(*this);
				return *this;
			}
			constexpr reverse_iterator
			operator+ (difference_type r) const
			{
				return reverse_iterator(p_ - r);
			}
			constexpr reverse_iterator
			operator- (difference_type r) const
			{
				return reverse_iterator(p_ + r);
			}
			explicit constexpr
			operator bool () const
			{
				return p_;
			}
			constexpr bool
			operator! () const
			{
				return !p_;
			}
		private:
			friend constexpr reverse_iterator
			operator+ (difference_type lhs, reverse_iterator const& rhs)
			{
				return rhs + lhs;
			}
			friend constexpr bool
			operator== (reverse_iterator const& lhs, reverse_iterator const& rhs)
			{
				return lhs.p_ == rhs.p_;
			}
			friend constexpr bool
			operator < (reverse_iterator const& lhs, reverse_iterator const& rhs)
			{
				return lhs.p_ < rhs.p_;
			}
		};	

		template < class Iter >
		void
		swap (ruin::iterator::reverse_iterator<Iter>& x, ruin::iterator::reverse_iterator<Iter>& y)
			noexcept(noexcept(x.swap(y)))
		{
			x.swap(y);
		}
	} // namespace iterator

	using ruin::iterator::swap;
} // namespace ruin

#endif // RUIN_ITERATOR_REVERSE_ITERATOR_HPP_INCLUDED

