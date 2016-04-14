
#ifndef RUIN_ITERATOR_INDEX_ITERATOR_HPP_INCLUDED
#define RUIN_ITERATOR_INDEX_ITERATOR_HPP_INCLUDED

#include <iterator>
#include <type_traits>
#include "ruin/container/traits.hpp"
#include "ruin/operators/additive.hpp"
#include "ruin/operators/decrementable.hpp"
#include "ruin/operators/equality_comparable.hpp"
#include "ruin/operators/incrementable.hpp"
#include "ruin/operators/less_than_comparable.hpp"
#include "ruin/utility/address_of.hpp"
#include "ruin/utility/swap.hpp"
#include "ruin/utility/value_holder.hpp"

namespace ruin
{
	namespace iterator
	{
		template < class Container >
		class index_iterator
			: public std::iterator
				< std::random_access_iterator_tag
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::value_type
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::difference_type
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::pointer
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::reference
				>
			, private ruin::additive<ruin::iterator::index_iterator<Container>, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::difference_type>
			, private ruin::incrementable<ruin::iterator::index_iterator<Container>>
			, private ruin::decrementable<ruin::iterator::index_iterator<Container>>
			, private ruin::equality_comparable<ruin::iterator::index_iterator<Container>>
			, private ruin::less_than_comparable<ruin::iterator::index_iterator<Container>>
		{
		public:
			typedef std::iterator
				< std::random_access_iterator_tag
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::value_type
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::difference_type
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::pointer
				, typename ruin::container::container_traits<typename std::remove_reference<Container>::type>::reference
				> base_type;

			typedef typename base_type::iterator_category iterator_category;
			typedef typename base_type::value_type value_type;
			typedef typename base_type::difference_type difference_type;
			typedef typename base_type::pointer pointer;
			typedef typename base_type::reference reference;

			typedef ruin::value_holder<Container> holder_type;
		private:
			holder_type holder_;
			difference_type index_;
		private:
			explicit constexpr
			index_iterator(holder_type const& r, difference_type i)
				: holder_(r)
				, index_(i)
			{ }
		public:
			index_iterator() = default;
			index_iterator(index_iterator const&) = default;
			explicit constexpr
			index_iterator(typename holder_type::param_type r, difference_type i)
				: holder_(r)
				, index_(i)
			{ }
			void
			swap(index_iterator& y)
				noexcept(noexcept(ruin::swap(index_iterator::holder_, y.holder_)) && noexcept(ruin::swap(index_iterator::index_, y.index_)))
			{
				ruin::swap(holder_, y.holder_);
				ruin::swap(index_, y.index_);
			}
			constexpr index_iterator
			next() const
			{
				return index_iterator(holder_, index_ + 1);
			}
			constexpr index_iterator
			prev() const
			{
				return index_iterator(holder_, index_ - 1);
			}
			index_iterator&
			operator=(index_iterator const& r)
			{
				index_iterator(r).swap(*this);
				return *this;
			}
		private:
			friend reference
			operator*(index_iterator& x)
			{
				return x.holder_.get()[x.index_];
			}
			friend constexpr reference
			operator*(index_iterator const& x)
			{
				return x.holder_.get()[x.index_];
			}
	public:
			constexpr pointer
			operator->() const
			{
				return ruin::address_of(holder_.get()[index_]);
			}
			constexpr reference
			operator[](difference_type i) const
			{
				return holder_.get()[index_ + i];
			}
			index_iterator&
			operator++()
			{
				next().swap(*this);
				return *this;
			}
			index_iterator&
			operator--()
			{
				prev().swap(*this);
				return *this;
			}
			constexpr index_iterator
			operator+(difference_type r) const
			{
				return index_iterator(holder_, index_ + r);
			}
			constexpr index_iterator
			operator-(difference_type r) const
			{
				return index_iterator(holder_, index_ - r);
			}
		private:
			friend constexpr index_iterator
			operator+(difference_type lhs, index_iterator const& rhs)
			{
				return rhs + lhs;
			}
			friend constexpr bool
			operator==(index_iterator const& lhs, index_iterator const& rhs)
			{
				return lhs.index_ == rhs.index_;
			}
			friend constexpr bool
			operator <(index_iterator const& lhs, index_iterator const& rhs)
			{
				return lhs.index_ < rhs.index_;
			}
		};

		template < class Container >
		void
		swap(ruin::iterator::index_iterator<Container>& x, ruin::iterator::index_iterator<Container>& y)
			noexcept(noexcept(x.swap(y)))
		{
			x.swap(y);
		}
	} // namespace iterator

	using ruin::iterator::swap;
} // namespace ruin

#endif // RUIN_ITERATOR_INDEX_ITERATOR_HPP_INCLUDED
