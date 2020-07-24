/*
 * optchain.h
 *
 *  Created on: May 2, 2020
 *      Author: georgios
 */

#ifndef OPTIONAL_PIPE_HPP_INCLUDED_
#define OPTIONAL_PIPE_HPP_INCLUDED_


#include <optional>
#include <type_traits>
#include <functional>

namespace detail
{
	/**
	 * helper type struct that creates the return type for the non-void
	 * functions. If the given type is already wrapped in an optional,
	 * then this type is returned. Otherwise the type is wrapped in an optional
	 * (after cvref was removed). If T=void, then the specialisation will
	 * produce SFINAE
	 */
	template<typename U>
	struct wrap_return_type
	{
		typedef std::optional<U> type;
	};

	template<typename V>
	struct wrap_return_type<std::optional<V>>
	{
		typedef std::optional<V> type;
	};

	template<>
	struct wrap_return_type<void>
	{
	};

	template<typename T>
	using wrap_return_type_t = typename wrap_return_type<T>::type;

	//!helper bool that checks whether a callable returns void when invoked with one argument
	template<typename F, typename T>
	constexpr bool invoke_result_is_void_v = std::is_same_v<void, std::invoke_result_t<F,T>>;
}

template<typename T, typename F>
auto operator|(const std::optional<T> & arg, F&& func)
-> detail::wrap_return_type_t<std::invoke_result_t<F, T>>
{
	if(arg.has_value())
	{
		return std::invoke(std::forward<F>(func), arg.value());
	}
	else
	{
		return std::nullopt;
	}
}

template<typename T, typename F>
auto operator|(const std::optional<T> & arg, F&& func)
-> std::enable_if_t<detail::invoke_result_is_void_v<F,T>, const std::optional<T>&>
{
	if(arg.has_value())
	{
		std::invoke(std::forward<F>(func), arg.value());
	}
	return arg;
}


#endif /* OPTIONAL_PIPE_HPP_INCLUDED_ */
