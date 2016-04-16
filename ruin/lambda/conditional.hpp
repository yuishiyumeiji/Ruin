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
			struct conditional0
				: public ruin::lambda::lambda_base
			{
			private:
				CondLE cond_;
				ThenLE then_;
				ElseLE else_;
			public:
				constexpr conditional0(CondLE const& c, ThenLE const& t, ElseLE const& e)
					: cond_(c), then_(t), else_(e)
				{ }
			public:
				template<class... Args>
				constexpr auto operator()(Args const&... args) const
				{
					return eval(std::make_tuple())(args...);
				}
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const
				{
					return cond_.eval(env) ? then_.eval(env) : else_.eval(env);
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<conditional0, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<conditional0, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
		}

		namespace detail
		{
			template<class CondLE, class ThenLE>
			struct else0_phase
			{
				CondLE const& cond_;
				ThenLE const& then_;
				else0_phase const& else_;
				constexpr else0_phase(CondLE const& c, ThenLE const& t)
					: cond_(c), then_(t), else_(*this)
				{ }
				template<class ElseLE>
				constexpr ruin::lambda::exp::conditional0<CondLE, ThenLE, ElseLE> operator[](ElseLE const& e) const
				{
					return {cond_, then_, e};
				}
			};
			template<class CondLE>
			struct then0_phase
			{
				CondLE const& cond_;
				then0_phase const& then_;
				constexpr then0_phase(CondLE const& c)
					: cond_(c), then_(*this)
				{ }
				template<class ThenLE>
				constexpr ruin::lambda::detail::else0_phase<CondLE, ThenLE> operator[](ThenLE const& t) const
				{
					return {cond_, t};
				}
			};
			struct if0_phase
			{
				template<class CondLE>
				constexpr ruin::lambda::detail::then0_phase<CondLE> operator[](CondLE const& c) const
				{
					return {c};
				}
			};
		}
		static constexpr ruin::lambda::detail::if0_phase if0{};

		namespace exp
		{
			template<class CondLE, class ThenLE, class ElseLE>
			struct conditional1
				: public ruin::lambda::lambda_base
			{
			private:
				CondLE cond_;
				ThenLE then_;
				ElseLE else_;
			public:
				constexpr conditional1(CondLE const& c, ThenLE const& t, ElseLE const& e)
					: cond_(c), then_(t), else_(e)
				{ }
			public:
				template<class... Args>
				constexpr auto operator()(Args const&... args) const
				{
					return eval(std::make_tuple())(args...);
				}
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const -> decltype(conditional1::then_.eval(env))
				{
					return cond_.eval(env) ? then_.eval(env) : else_.eval(env);
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<conditional1, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<conditional1, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
		}

		namespace detail
		{
			template<class CondLE, class ThenLE>
			struct else1_phase
			{
				CondLE const& cond_;
				ThenLE const& then_;
				else1_phase const& else_;
				constexpr else1_phase(CondLE const& c, ThenLE const& t)
					: cond_(c), then_(t), else_(*this)
				{ }
				template<class ElseLE>
				constexpr ruin::lambda::exp::conditional1<CondLE, ThenLE, ElseLE> operator[](ElseLE const& e) const
				{
					return {cond_, then_, e};
				}
			};
			template<class CondLE>
			struct then1_phase
			{
				CondLE const& cond_;
				then1_phase const& then_;
				constexpr then1_phase(CondLE const& c)
					: cond_(c), then_(*this)
				{ }
				template<class ThenLE>
				constexpr ruin::lambda::detail::else1_phase<CondLE, ThenLE> operator[](ThenLE const& t) const
				{
					return {cond_, t};
				}
			};
			struct if1_phase
			{
				template<class CondLE>
				constexpr ruin::lambda::detail::then1_phase<CondLE> operator[](CondLE const& c) const
				{
					return {c};
				}
			};
		}
		static constexpr ruin::lambda::detail::if1_phase if1{};

		namespace exp
		{
			template<class CondLE, class ThenLE, class ElseLE>
			struct conditional2
				: public ruin::lambda::lambda_base
			{
			private:
				CondLE cond_;
				ThenLE then_;
				ElseLE else_;
			public:
				constexpr conditional2(CondLE const& c, ThenLE const& t, ElseLE const& e)
					: cond_(c), then_(t), else_(e)
				{ }
			public:
				template<class... Args>
				constexpr auto operator()(Args const&... args) const
				{
					return eval(std::make_tuple())(args...);
				}
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const -> decltype(conditional2::else_.eval(env))
				{
					return cond_.eval(env) ? then_.eval(env) : else_.eval(env);
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<conditional2, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<conditional2, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
		}

		namespace detail
		{
			template<class CondLE, class ThenLE>
			struct else2_phase
			{
				CondLE const& cond_;
				ThenLE const& then_;
				else2_phase const& else_;
				constexpr else2_phase(CondLE const& c, ThenLE const& t)
					: cond_(c), then_(t), else_(*this)
				{ }
				template<class ElseLE>
				constexpr ruin::lambda::exp::conditional2<CondLE, ThenLE, ElseLE> operator[](ElseLE const& e) const
				{
					return {cond_, then_, e};
				}
			};
			template<class CondLE>
			struct then2_phase
			{
				CondLE const& cond_;
				then2_phase const& then_;
				constexpr then2_phase(CondLE const& c)
					: cond_(c), then_(*this)
				{ }
				template<class ThenLE>
				constexpr ruin::lambda::detail::else2_phase<CondLE, ThenLE> operator[](ThenLE const& t) const
				{
					return {cond_, t};
				}
			};
			struct if2_phase
			{
				template<class CondLE>
				constexpr ruin::lambda::detail::then2_phase<CondLE> operator[](CondLE const& c) const
				{
					return {c};
				}
			};
		}
		static constexpr ruin::lambda::detail::if2_phase if2{};
	}
}

#endif // RUIN_LAMBDA_CONDITIONAL_HPP_INCLUDED

