﻿//
// Copyright (C) 2011-2016 Yuishi Yumeiji.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RUIN_LAMBDA_LAMBDA_HPP_INCLUDED
#define RUIN_LAMBDA_LAMBDA_HPP_INCLUDED

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include "ruin/utility/index_tuple.hpp"

namespace ruin
{
	namespace lambda
	{
		namespace detail
		{
			template<std::size_t BV, class LE>
			struct binder
			{
				LE le_;
				constexpr binder(LE const& le)
					: le_(le)
				{ }
			};

			template<std::size_t, class, int = 0>
			struct get_impl;
			template<std::size_t VID, class Head, class... Tail, int I>
			struct get_impl<VID, std::tuple<Head, Tail...>, I>
				: public ruin::lambda::detail::get_impl<VID, std::tuple<Tail...>, I + 1>
			{ };
			template<std::size_t VID, class LE, class... Tail, int I>
			struct get_impl<VID, std::tuple<ruin::lambda::detail::binder<VID, LE>, Tail...>, I>
				: public std::integral_constant<std::size_t, I>
			{ };
			template<std::size_t VID, int I>
			struct get_impl<VID, std::tuple<>, I>
				: public std::integral_constant<std::size_t, -1>
			{ };
			template<std::size_t VID, class EnvList>
			constexpr auto get(EnvList const& env)
			{
				return std::get<ruin::lambda::detail::get_impl<VID, EnvList>::value>(env).le_;
			}
		}

		class lambda_base
		{ };
		template<class X>
		struct is_lambda
			: public std::is_base_of<ruin::lambda::lambda_base, X>
		{ };
		template<class>
		struct is_binder
			: public std::integral_constant<bool, false>
		{ };
		template<std::size_t VID, class LE>
		struct is_binder<ruin::lambda::detail::binder<VID, LE>>
			: public std::integral_constant<bool, true>
		{ };
	
		namespace exp
		{
			template<std::size_t>
			struct variable;
			template<class, class>
			struct closure;
			template<class, class>
			struct apply;
			template<class, class>
			struct abstract;
			template<class>
			struct constant;
			
			//          X.eval(env)       => (the value of (the first) X in env).eval(env)
			// let[env].in[e].eval(env')  => e.eval(env, env')
			//   e[a1,...,an].eval(env)   => e.eval(env)(a1.eval(env),...,an.eval(env))
			//   lambda(X)[e].eval(env)   => lambda(Y)[let[env].in[e[X->Y]]]
			//    constant(v).eval(env)   => v
			//
			//    let[env].in[e](v...)    => let[env].in[e].eval()(v...)
			//     e[a1,...,an](v...)     => e[a1,...,an].eval()(v...)
			//      lambda(X)[e](v)       => e.eval(X == v)
			//     constant(u)(v...)      => u(v...)
			//
			
			template<std::size_t VID>
			struct variable
				: public ruin::lambda::lambda_base
			{
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const
				{
					return ruin::lambda::detail::get<VID>(env).eval(env);
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<variable, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<variable, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<class EnvList, class LE>
			struct closure
				: public ruin::lambda::lambda_base
			{
			private:
				EnvList env_;
				LE le_;
			public:
				constexpr closure(EnvList const& env, LE const& le)
					: env_(env), le_(le)
				{ }
			public:
				template<class... Args>
				constexpr auto operator()(Args const&... args) const
				{
					return eval(std::make_tuple())(args...);
				}
				template<class EnvList2>
				constexpr auto eval(EnvList2 const& env2) const
				{
					return le_.eval(std::tuple_cat(env_, env2));
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<closure, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<closure, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<class LE, class ArgList>
			struct apply
				: public ruin::lambda::lambda_base
			{
			private:
				LE le_;
				ArgList args_;
			public:
				constexpr apply(LE const& le, ArgList const& args)
					: le_(le), args_(args)
				{ }
			private:
				template<class EnvList, std::size_t... Indices>
				static constexpr auto eval_impl(LE const& le, ArgList const& args, EnvList const& env, ruin::index_tuple<Indices...>)
				{
					return le.eval(env)(std::get<Indices>(args).eval(env)...);
				}
			public:
				template<class... Args>
				constexpr auto operator()(Args const&... args) const
				{
					return eval(std::make_tuple())(args...);
				}
				template<class EnvList>
				constexpr auto eval(EnvList const& env) const
				{
					return eval_impl(le_, args_, env, ruin::index_range<0, std::tuple_size<ArgList>::value>::make());
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<apply, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<apply, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<class BVs, class LE>
			struct abstract
				: public ruin::lambda::lambda_base
			{
			private:
				LE le_;
			public:
				constexpr abstract(LE const& le)
					: le_(le)
				{ }
			private:
				template<std::size_t I, class... Args>
				static constexpr auto make_binder_from_tuple(std::tuple<Args...> const& args)
				{
					return (std::get<I>(BVs{}) == std::get<I>(args));
				}
				template<std::size_t... Indices, class... Args>
				static constexpr auto make_tuple_of_binders(std::tuple<Args...> const& args, ruin::index_tuple<Indices...>)
				{
					return std::make_tuple(make_binder_from_tuple<Indices>(args)...);
				}
			public:
				template<class... Args>
				constexpr auto operator()(Args const&... args) const
				{
					static_assert(std::tuple_size<BVs>::value == sizeof...(Args), "invalid number of arguments");
					return le_.eval(make_tuple_of_binders(std::make_tuple(args...), ruin::index_range<0, sizeof...(Args)>::make()));
				}
				// substitution [X->X'] is unimplemented
				template<class EnvList>
				constexpr ruin::lambda::exp::abstract<BVs, closure<EnvList, LE>> eval(EnvList const& env) const
				{
					return {{env, le_}};
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<abstract, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<abstract, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
			template<class T>
			struct constant
				: public ruin::lambda::lambda_base
			{
			private:
				T value_;
			public:
				constexpr constant(T const& t)
					: value_(t)
				{ }
			public:
				template<class... Args>
				constexpr auto operator()(Args const&... args) const
				{
					return value_(args...);
				}
				template<class EnvList>
				constexpr T eval(EnvList const&) const
				{
					return value_;
				}
				template<class Arg>
				constexpr ruin::lambda::exp::apply<constant, std::tuple<Arg>> operator[](Arg const& arg) const
				{
					return {*this, std::make_tuple(arg)};
				}
				template<class... Args>
				constexpr ruin::lambda::exp::apply<constant, std::tuple<Args...>> operator[](std::tuple<Args...> const& args) const
				{
					return {*this, args};
				}
			};
		}
	
		template<class T>
		constexpr ruin::lambda::exp::constant<typename std::decay<T>::type> val(T t)
		{
			return {t};
		}
	
		namespace detail
		{
			template<class EnvList>
			struct in_phase
			{
				EnvList const& env_;
				in_phase const& in;
				constexpr in_phase(EnvList const& env)
					: env_(env), in(*this)
				{ }
				template<class LE>
				constexpr ruin::lambda::exp::closure<EnvList, LE> operator[](LE const& le) const
				{
					return {env_, le};
				}
			};
			struct let_phase
			{
				template<class Binder>
				constexpr ruin::lambda::detail::in_phase<std::tuple<Binder>> operator[](Binder const& bdr) const
				{
					return {std::make_tuple(bdr)};
				}
				template<class... Envs>
				constexpr ruin::lambda::detail::in_phase<std::tuple<Envs...>> operator[](std::tuple<Envs...> const& bdrs) const
				{
					return {bdrs};
				}
			};
		}
		static constexpr ruin::lambda::detail::let_phase let{};
		
		namespace detail
		{
			template<std::size_t... BVIDs>
			struct lambda_phase
			{
				template<class LE>
				constexpr ruin::lambda::exp::abstract<std::tuple<ruin::lambda::exp::variable<BVIDs>...>, LE> operator[](LE const& le) const
				{
					return {le};
				}
			};
		}
		template<std::size_t... BVIDs>
		constexpr ruin::lambda::detail::lambda_phase<BVIDs...> lambda(ruin::lambda::exp::variable<BVIDs>...)
		{
			return {};
		}
		
		namespace detail
		{
			template<class T, bool = ruin::lambda::is_lambda<T>::value>
			struct LEify
			{
				typedef T type;
				static constexpr T go(T const& x)
				{
					return x;
				}
			};
			template<class T>
			struct LEify<T, false>
			{
				typedef ruin::lambda::exp::constant<T> type;
				static constexpr type go(T const& x)
				{
					return {x};
				}
			};
		}
		template<class T>
		constexpr typename ruin::lambda::detail::LEify<T>::type leify(T const& t)
		{
			return ruin::lambda::detail::LEify<T>::go(t);
		}
		
		template<std::size_t BVID, class T>
		constexpr ruin::lambda::detail::binder<BVID, typename ruin::lambda::detail::LEify<T>::type> operator==(ruin::lambda::exp::variable<BVID>, T t)
		{
			return {ruin::lambda::detail::LEify<T>::go(t)};
		}
		
		template<class T, class U, class = typename std::enable_if<(ruin::lambda::is_lambda<T>::value && ruin::lambda::is_lambda<U>::value) || (ruin::lambda::is_binder<T>::value && ruin::lambda::is_binder<U>::value)>::type>
		constexpr auto operator,(T const& t, U const& u)
		{
			return std::make_tuple(t, u);
		}
		template<class... List, class T, class = typename std::enable_if<ruin::lambda::is_lambda<T>::value || ruin::lambda::is_binder<T>::value>::type>
		constexpr auto operator,(std::tuple<List...> const& lis, T const& t)
		{
			return std::tuple_cat(lis, std::make_tuple(t));
		}
	}
}

#endif // RUIN_LAMBDA_LAMBDA_HPP_INCLUDED

