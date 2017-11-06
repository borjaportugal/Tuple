/*!
\author Borja Portugal Martin
*/


#pragma once

#include <cstdlib>	// std::size_t

template <typename ... Ts>
class tuple;

template <>
class tuple<> {};

template <typename T, typename ... Ts>
class tuple<T, Ts ...> : public tuple<Ts...>
{
public:
	using base = tuple<Ts...>;
	using value_type = T;

public:
	tuple() = default;
	tuple(T && t, Ts && ... vs)
		: base{ std::forward<Ts>(vs) ... }
		, m_value{ std::forward<T>(t) }
	{}

public:
	T m_value;
};

namespace impl
{
	template <std::size_t I, typename T, typename ... Ts>
	struct tuple_get_impl
	{
		static auto & get(tuple<T, Ts ...> & t)
		{
			return tuple_get_impl<I - 1, Ts ...>::get(static_cast<tuple<Ts ...> &>(t));
		}
	};

	template <typename T, typename ... Ts>
	struct tuple_get_impl<0, T, Ts ...>
	{
		static auto & get(tuple<T, Ts ...> & t)
		{
			return t.m_value;
		}
	};

	template <std::size_t I, typename ... Ts>
	auto & get(tuple<Ts ...> & t)
	{
		return tuple_get_impl<I, Ts...>::get(t);
	}
}

template <std::size_t I, typename ... Ts>
auto & get(tuple<Ts ...> & t)
{
	return ::impl::get<I>(t);
}


