
#ifndef RUIN_ITERATOR_VALUE_ITERATOR_HPP_INCLUDED
#define RUIN_ITERATOR_VALUE_ITERATOR_HPP_INCLUDED

#include <cstddef>
#include <iterator>
#include "ruin/operators/additive.hpp"
#include "ruin/operators/decrementable.hpp"
#include "ruin/operators/equality_comparable.hpp"
#include "ruin/operators/incrementable.hpp"
#include "ruin/operators/less_than_comparable.hpp"
#include "ruin/utility/declval.hpp"
#include "ruin/utility/forward.hpp"
#include "ruin/utility/swap.hpp"

namespace ruin
{
	namespace iterator
	{
		template < class T >
		class value_iterator
			: public std::iterator
				< std::random_access_iterator_tag
				, T
				, std::ptrdiff_t
				, void
				, T
				>
			, private ruin::additive<ruin::iterator::value_iterator<T>, std::ptrdiff_t>
			, private ruin::incrementable<ruin::iterator::value_iterator<T>>
			, private ruin::decrementable<ruin::iterator::value_iterator<T>>
			, private ruin::equality_comparable<ruin::iterator::value_iterator<T>>
			, private ruin::less_than_comparable<ruin::iterator::value_iterator<T>>
		{
		private:
			typedef std::iterator
				< std::random_access_iterator_tag
					, T
				, std::ptrdiff_t
				, void
				, T
				> base_type;
		public:
			typedef typename base_type::iterator_category iterator_category;
			typedef typename base_type::value_type value_type;
			typedef typename base_type::difference_type difference_type;
			typedef typename base_type::reference reference;
			typedef typename base_type::pointer pointer;
		private:
			T x_;
			T diff_;
		public:
			value_iterator(value_iterator const&) = default;
			explicit constexpr
			value_iterator(T const& x = {}, T const& diff = 1)
				: x_(x)
				, diff_(diff)
			{ }
		public:
			void
			swap(value_iterator& y)
				noexcept(noexcept(ruin::swap(value_iterator::x_, y.x_)) && noexcept(ruin::swap(diff_, y.diff_)))
			{
				ruin::swap(x_, y.x_);
				ruin::swap(diff_, y.diff_);
			}
			constexpr value_iterator
			next() const
			{
				return value_iterator(x_ + diff_, diff_);
			}
			constexpr value_iterator
			prev() const
			{
				return value_iterator(x_ - diff_, diff_);
			}
			value_iterator&
			operator=(value_iterator const& r)
			{
				value_iterator(r).swap(*this);
				return *this;
			}
			constexpr reference
			operator*() const
			{
				return x_;
			}
			constexpr reference
			operator[](difference_type i) const
			{
				return x_ + diff_ * i;
			}
			value_iterator&
			operator++()
			{
				next().swap(*this);
				return *this;
			}
			value_iterator&
			operator--()
			{
				prev().swap(*this);
				return *this;
			}
			constexpr value_iterator
			operator+(difference_type r) const
			{
				return value_iterator(x_ + diff_ * r, diff_);
			}
			constexpr value_iterator
			operator-(difference_type r) const
			{
				return value_iterator(x_ - diff_ * r, diff_);
			}
			explicit constexpr
			operator bool() const
			{
				return true;
			}
			constexpr bool
			operator!() const
			{
				return false;
			}
		private:
			friend constexpr value_iterator
			operator+(difference_type lhs, value_iterator const& rhs)
			{
				return rhs + lhs;
			}
			friend constexpr bool
			operator==(value_iterator const& lhs, value_iterator const& rhs)
			{
				return lhs.x_ == rhs.x_;
			}
			friend constexpr bool
			operator<(value_iterator const& lhs, value_iterator const& rhs)
			{
				return lhs.x_ < rhs.x_;
			}
		};

		template < class T >
		void
		swap(ruin::iterator::value_iterator<T>& x, ruin::iterator::value_iterator<T>& y)
			noexcept(noexcept(x.swap(y)))
		{
			x.swap(y);
		}
	} // namespace iterator

	using ruin::iterator::swap;
} // namespace ruin
	
#endif // RUIN_ITERATOR_VALUE_ITERATOR_HPP_INCLUDED
