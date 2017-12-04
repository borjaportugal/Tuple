/*!
\author Borja Portugal Martin

GitHub: https://github.com/borjaportugal

This file is subject to the license terms in the LICENSE file
found in the top-level directory of this distribution.
*/

#pragma once

#include <cstdlib>	// std::size_t
#include <type_traits>	// std::true_type, std::false_type

template <typename ... Ts>
class tuple;

template <>
class tuple<> 
{
public:
	bool operator==(const tuple &) const { return true; }
	bool operator!=(const tuple & rhs) const { return !operator==(rhs); }
};

template <typename T, typename ... Ts>
class tuple<T, Ts ...> : private tuple<Ts...>
{
public:
	using Base = tuple<Ts...>;
	using value_type = T;

public:
	tuple() = default;
	template <typename U, typename ... Us>
	tuple(U && t, Us && ... vs)
		: Base{ std::forward<Us>(vs) ... }
		, _value{ std::forward<U>(t) }
	{}

	template <typename U, typename ... Us, typename ... Tuples>
	tuple(const tuple<U, Us ...> & t, const Tuples & ... tuples)
		: Base{ (const tuple<Us...>&)t, tuples ... }
		, _value{ get<0>(t) }
	{}

	template <typename U, typename ... Tuples>
	tuple(const tuple<U> & t, const Tuples & ... tuples)
		: Base{ tuples ... }
		, _value{ get<0>(t) }
	{}

	template <typename U, typename ... Us>
	tuple<Ts...> & operator=(const tuple<U, Us...> & rhs)
	{
		Base::operator=((const tuple<Us...> &)(rhs));
		_value = rhs._value;
		return *this;
	}

	bool operator==(const tuple & rhs) const
	{
		return _value == rhs._value && Base::operator==(rhs);
	}
	bool operator!=(const tuple & rhs) const
	{
		return !operator==(rhs);
	}


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
	struct tuple_element;

	template <std::size_t I, typename T, typename ... Ts>
	struct tuple_element<I, tuple<T, Ts ...>>
		: public tuple_element<I - 1, tuple<Ts ...>>
	{};

	template <typename T, typename ... Ts>
	struct tuple_element<0, tuple<T, Ts ...>>
	{
		using type = T;
	};

	// tuple_cat_type
	template <typename ... Tuples>
	struct tuple_cat_type;
	
	template <typename ... Ts, typename ... Currents, typename ... Tuples>
	struct tuple_cat_type<tuple<Ts ...>, tuple<Currents ...>, Tuples ...>
		: public tuple_cat_type<tuple<Ts ..., Currents ...>, Tuples ...>
	{};

	template <typename ... Ts>
	struct tuple_cat_type<tuple<Ts ...>>
	{
		using type = tuple<Ts ...>;
	};

	// make_from_tuple
	template <typename T, typename Tuple, std::size_t ... Is>
	T make_from_tuple(Tuple & t, std::integer_sequence<std::size_t, Is...>)
	{
		return T(get<Is>(t) ...);
	}

	template <typename T>
	struct tuple_ref_wrap
	{
		tuple_ref_wrap(T & ref)
			: _ref(ref)
		{}

		operator T &() { return _ref; }
		operator T &() const { return _ref; }

		T & _ref;
		using type = T &;
	};

	template <typename T>
	struct resolve_ref_warp
	{
		using type = T;
	};

	template <typename T>
	struct resolve_ref_warp<tuple_ref_wrap<T>>
	{
		using type = T &;
	};

	template <typename T>
	using resolve_ref_warp_t = typename resolve_ref_warp<T>::type;

	struct tuple_ignore_t 
	{
		template <typename T>
		tuple_ignore_t & operator=(const T &)
		{
			return *this;
		}
	};
}

static ::impl::tuple_ignore_t ignore;

// is_tuple 
template <typename T>
struct is_tuple : std::false_type {};
template <typename ... Ts>
struct is_tuple<tuple<Ts...>> : std::true_type {};

// tuple_element_t
template <std::size_t I, typename Tuple>
using tuple_element_t = typename ::impl::tuple_element<I, Tuple>::type;

// tuple_cat_type_t
template <typename ... Tuples>
using tuple_cat_type_t = typename ::impl::tuple_cat_type<Tuples ...>::type;



template <typename T>
::impl::tuple_ref_wrap<T> tuple_ref(T & t)
{
	return{ t };
}

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

template <typename ... Ts>
auto make_tuple(Ts && ... vs)
{
	using tuple_type = tuple<::impl::resolve_ref_warp_t<Ts> ...>;
	return tuple_type{ std::forward<::impl::resolve_ref_warp_t<Ts>>(vs) ... };
}

template <typename ... Tuples>
auto tuple_cat(const Tuples & ... tuples)
{
	tuple_cat_type_t<Tuples ...> new_tuple(tuples ...);
	return new_tuple;
}

template <typename ... Ts>
auto tie(Ts && ... vs)
{
	return tuple<Ts ...>{ std::forward<Ts>(vs) ... };
}

template <typename T, typename ... Ts>
T make_from_tuple(tuple<Ts ...> & t)
{
	return ::impl::make_from_tuple<T>(t, std::make_integer_sequence<std::size_t, sizeof...(Ts)>{});
}

template <typename T, typename ... Ts>
T make_from_tuple(const tuple<Ts ...> & t)
{
	return ::impl::make_from_tuple<T>(t, std::make_integer_sequence<std::size_t, sizeof...(Ts)>{});
}


