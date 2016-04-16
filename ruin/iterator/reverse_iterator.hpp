//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_ITERATOR_REVERSE_ITERATOR_HPP_INCLUDED
#define RUIN_ITERATOR_REVERSE_ITERATOR_HPP_INCLUDED

#include <iterator>
#include <utility>
#include "ruin/operators/additive.hpp"
#include "ruin/operators/decrementable.hpp"
#include "ruin/operators/equality_comparable.hpp"
#include "ruin/operators/incrementable.hpp"
#include "ruin/operators/less_than_comparable.hpp"

namespace ruin
{
	template<class Iter>
	class reverse_iterator
		: public std::iterator
			< typename std::iterator_traits<Iter>::iterator_category
			, typename std::iterator_traits<Iter>::value_type
			, typename std::iterator_traits<Iter>::difference_type
			, typename std::iterator_traits<Iter>::pointer
			, typename std::iterator_traits<Iter>::reference
			>
		, private ruin::additive<ruin::reverse_iterator<Iter>, typename ruin::iterator_traits<Iter>::difference_type>
		, private ruin::incrementable<ruin::reverse_iterator<Iter>>
		, private ruin::decrementable<ruin::reverse_iterator<Iter>>
		, private ruin::equality_comparable<ruin::reverse_iterator<Iter>>
		, private ruin::less_than_comparable<ruin::reverse_iterator<Iter>>
	{
		template<class>
		friend class ruin::reverse_iterator;
	private:
		typedef std::iterator_traits<Iter> traits_type;
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
		explicit constexpr reverse_iterator (iterator_type p)
			: p_(p), deref_(p-1)
		{ }
	public:
		void swap (reverse_iterator& y)
		{
			std::swap(p_, y.p_);
			std::swap(deref_, y.deref_);
		}
		constexpr reverse_iterator next() const
		{
			return reverse_iterator(p_ - 1);
		}
		constexpr reverse_iterator prev() const
		{
			return reverse_iterator(p_ + 1);
		}
		reverse_iterator& operator=(reverse_iterator const& r)
		{
			reverse_iterator(r).swap(*this);
			return *this;
		}
		constexpr reference operator*() const
		{
			return *deref_;
		}
		constexpr pointer operator->() const
		{
			return &*deref_;
		}
		constexpr reference operator[](difference_type i) const
		{
			return *(deref_ - i);
		}
		reverse_iterator& operator++()
		{
			next().swap(*this);
			return *this;
		}
		reverse_iterator& operator--()
		{
			prev().swap(*this);
			return *this;
		}
		constexpr reverse_iterator operator+(difference_type r) const
		{
			return reverse_iterator(p_ - r);
		}
		constexpr reverse_iterator operator-(difference_type r) const
		{
			return reverse_iterator(p_ + r);
		}
		explicit constexpr operator bool() const
		{
			return p_;
		}
		constexpr bool operator!() const
		{
			return !p_;
		}
	private:
		friend constexpr reverse_iterator operator+(difference_type l, reverse_iterator const& r)
		{
			return r + l;
		}
		friend constexpr bool operator==(reverse_iterator const& l, reverse_iterator const& r)
		{
			return l.p_ == r.p_;
		}
		friend constexpr bool operator<(reverse_iterator const& l, reverse_iterator const& r)
		{
			return l.p_ < r.p_;
		}
	};	
}

#endif // RUIN_ITERATOR_REVERSE_ITERATOR_HPP_INCLUDED

