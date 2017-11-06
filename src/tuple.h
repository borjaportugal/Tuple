/*!
\author Borja Portugal Martin
*/

#pragma once

#include <cstdlib>	// std::size_t
#include <type_traits>	// std::true_type, std::false_type

template <typename ... Ts>
class tuple;

template <>
class tuple<> {};

template <typename T, typename ... Ts>
class tuple<T, Ts ...> : private tuple<Ts...>
{
public:
	using base = tuple<Ts...>;
	using value_type = T;

public:
	tuple() = default;
	tuple(T && t, Ts && ... vs)
		: base{ std::forward<Ts>(vs) ... }
		, _value{ std::forward<T>(t) }
	{}

	T _value;
};

namespace impl
{
	// tuple_get_impl
	template <std::size_t I, typename T, typename ... Ts>
	struct tuple_get_impl
	{
		static auto & get(tuple<T, Ts ...> & t)
		{
			return tuple_get_impl<I - 1, Ts ...>::get((tuple<Ts ...> &)(t));
		}
		static const auto & get(const tuple<T, Ts ...> & t)
		{
			return tuple_get_impl<I - 1, Ts ...>::get((const tuple<Ts ...> &)(t));
		}
	};

	template <typename T, typename ... Ts>
	struct tuple_get_impl<0, T, Ts ...>
	{
		static auto & get(tuple<T, Ts ...> & t)
		{
			return t._value;
		}
		static const auto & get(const tuple<T, Ts ...> & t)
		{
			return t._value;
		}
	};

	// tuple_element_impl
	template <std::size_t I, typename T>
	struct tuple_element_impl;

	template <std::size_t I, typename T, typename ... Ts>
	struct tuple_element_impl<I, tuple<T, Ts ...>>
		: public tuple_element_impl<I - 1, tuple<Ts ...>>
	{};

	template <typename T, typename ... Ts>
	struct tuple_element_impl<0, tuple<T, Ts ...>>
	{
		using type = T;
	};
}

// is_tuple 
template <typename T>
struct is_tuple : std::false_type {};
template <typename ... Ts>
struct is_tuple<tuple<Ts...>> : std::true_type {};

// tuple_element_t
template <std::size_t I, typename Tuple>
using tuple_element_t = typename ::impl::tuple_element_impl<I, Tuple>::type;

// get
template <std::size_t I, typename ... Ts>
auto & get(tuple<Ts ...> & t)
{
	return ::impl::tuple_get_impl<I, Ts...>::get(t);
}
template <std::size_t I, typename ... Ts>
const auto & get(const tuple<Ts ...> & t)
{
	return ::impl::tuple_get_impl<I, Ts...>::get(t);
}

// make_tuple
template <typename ... Ts>
auto make_tuple(Ts && ... vs)
{
	return tuple<Ts ...>{ std::forward<Ts>(vs) ... };
}
