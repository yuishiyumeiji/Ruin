//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_LAMBDA_CONDITIONAL_HPP_INCLUDED
#define RUIN_LAMBDA_CONDITIONAL_HPP_INCLUDED

#include <tuple>
#include "ruin/lambda/lambda.hpp"

namespace ruin
{
	namespace lambda
	{
		namespace exp
		{
			template<class CondLE, class ThenLE, class ElseLE>
			struct conditional
				: public ruin::lambda::lambda_base
			{
			private:
				CondLE cond_;
				ThenLE then_;
				ElseLE else_;
			public:
				constexpr conditional(CondLE const& c, ThenLE const& t, ElseLE const& e)
					: cond_(c), then_(t), else_(e)
				{ }
			public:
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const
				{
					return cond_.eval(env) ? then_.eval(env) : else_.eval(env);
				}
				template<class Arg>
				constexpr apply<conditional, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr apply<conditional, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
		}
		
		namespace detail
		{
			template<class CondLE, class ThenLE>
			struct if_impl1
			{
				CondLE const& cond_;
				ThenLE const& then_;
				if_impl1 const& else_;
				constexpr if_impl1(CondLE const& c, ThenLE const& t)
					: cond_(c), then_(t), else_(*this)
				{ }
				template<class ElseLE>
				constexpr ruin::lambda::exp::conditional<CondLE, ThenLE, ElseLE> operator[](ElseLE const& e) const
				{
					return {cond_, then_, e};
				}
			};
			template<class CondLE>
			struct if_impl2
			{
				CondLE const& cond_;
				if_impl2 const& then_;
				constexpr if_impl2(CondLE const& c)
					: cond_(c), then_(*this)
				{ }
				template<class ThenLE>
				constexpr ruin::lambda::detail::if_impl1<CondLE, ThenLE> operator[](ThenLE const& t) const
				{
					return {cond_, t};
				}
			};
			struct if_impl3
			{
				template<class CondLE>
				constexpr ruin::lambda::detail::if_impl2<CondLE> operator[](CondLE const& c) const
				{
					return {c};
				}
			};
		}
		static constexpr ruin::lambda::detail::if_impl3 if_{};
	}
}

#endif // RUIN_LAMBDA_CONDITIONAL_HPP_INCLUDED

