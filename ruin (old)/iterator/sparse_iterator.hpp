
#ifndef RUIN_ITERATOR_SPARSE_ITERATOR_HPP_INCLUDED
#define RUIN_ITERATOR_SPARSE_ITERATOR_HPP_INCLUDED

#include <iterator>
#include <type_traits>
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
		class sparse_iterator
			: public std::iterator
				< typename ruin::iterator::iterator_traits<Iter>::iterator_category
				, typename ruin::iterator::iterator_traits<Iter>::value_type
				, typename ruin::iterator::iterator_traits<Iter>::difference_type
				, typename ruin::iterator::iterator_traits<Iter>::pointer
				, typename ruin::iterator::iterator_traits<Iter>::reference
				>
			, private ruin::additive<ruin::iterator::sparse_iterator<Iter>, typename ruin::iterator::iterator_traits<Iter>::difference_type>
			, private ruin::incrementable<ruin::iterator::sparse_iterator<Iter>>
			, private ruin::decrementable<ruin::iterator::sparse_iterator<Iter>>
			, private ruin::equality_comparable<ruin::iterator::sparse_iterator<Iter>>
			, private ruin::less_than_comparable<ruin::iterator::sparse_iterator<Iter>>
		{
			template < class >
			friend class ruin::iterator::sparse_iterator;
		private:
			typedef ruin::iterator::iterator_traits<Iter> traits_type;
		public:
			typedef Iter iterator_type;
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type value_type;
			typedef typename traits_type::difference_type difference_type;
			typedef typename traits_type::pointer pointer;
			typedef typename traits_type::reference reference;
		protected:
			Iter p_;
		private:
			difference_type diff_;
		public:
			sparse_iterator() = default;
			sparse_iterator(sparse_iterator const&) = default;
			explicit constexpr
			sparse_iterator(iterator_type p, difference_type d = 1)
				: p_(p)
				, diff_(d)
			{ }
			void
			swap(sparse_iterator& y)
				noexcept(noexcept(ruin::swap(sparse_iterator::p_, y.p_))
				&& noexcept(ruin::swap(sparse_iterator::diff_, y.diff_))
				)
			{
				ruin::swap(p_, y.p_);
				ruin::swap(diff_, y.diff_);
			}
			constexpr sparse_iterator
			next() const
			{
				return sparse_iterator(p_ + diff_, diff_);
			}
			constexpr sparse_iterator
			prev() const
			{
				return sparse_iterator(p_ - diff_, diff_);
			}
			sparse_iterator&
			operator=(sparse_iterator const& r)
			{
				sparse_iterator(r).swap(*this);
				return *this;
			}
			constexpr reference
			operator*() const
			{
				return *p_;
			}
			constexpr pointer
			operator->() const
			{
				return ruin::address_of(*p_);
			}
			constexpr reference
			operator[](difference_type i) const
			{
				return *(p_ + diff_ * i);
			}
			sparse_iterator&
			operator++()
			{
				next().swap(*this);
				return *this;
			}
			sparse_iterator&
			operator--()
			{
				prev().swap(*this);
				return *this;
			}
			constexpr sparse_iterator
			operator+(difference_type r) const
			{
				return sparse_iterator(p_ + diff_ * r, diff_);
			}
			constexpr sparse_iterator
			operator-(difference_type r) const
			{
				return sparse_iterator(p_ - diff_ * r, diff_);
			}
			explicit constexpr
			operator bool() const
			{
				return p_;
			}
			constexpr bool
			operator!() const
			{
				return !p_;
			}
		private:
			friend constexpr sparse_iterator
			operator+(difference_type lhs, sparse_iterator const& rhs)
			{
				return rhs + lhs;
			}
			friend constexpr bool
			operator==(sparse_iterator const& lhs, sparse_iterator const& rhs)
			{
				return lhs.p_ == rhs.p_;
			}
			friend constexpr bool
			operator<(sparse_iterator const& lhs, sparse_iterator const& rhs)
			{
				return lhs.p_ < rhs.p_;
			}
		};

		template < class Iter >
		void
		swap(ruin::iterator::sparse_iterator<Iter>& x, ruin::iterator::sparse_iterator<Iter>& y)
			noexcept(noexcept(x.swap(y)))
		{
			x.swap(y);
		}
	} // namespace iterator

	using ruin::iterator::swap;
} // namespace ruin

#endif // RUIN_ITERATOR_SPARSE_ITERATOR_HPP_INCLUDED
