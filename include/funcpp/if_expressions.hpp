//
// Created by georgios on 24.07.20.
//

#ifndef FUNCPP_IF_EXPRESSIONS_HPP
#define FUNCPP_IF_EXPRESSIONS_HPP

#include <type_traits>

namespace funcpp {

    template <typename T>
    using remove_cvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    template<typename T> class If_Then;
    template <typename T>
    class If_Then_Else_If;

    class If
    {
    public:
        If(const If& other) = delete;
        If& operator=(const If &) = delete;

        constexpr explicit If(bool condition):condition_(condition){}

        template<typename T>
        constexpr If_Then<remove_cvref_t<T>> Then(T&& t) const &&
        {
            return If_Then<remove_cvref_t<T>>(condition_, std::forward<T>(t));
        }

    private:
        bool condition_;
    };


    template<typename T>
    class If_Then_Else_If
    {
    public:

        If_Then_Else_If & operator=(const If_Then_Else_If<T> & other)=delete;
        If_Then_Else_If(const If_Then_Else_If<T> & other)=delete;

        constexpr If_Then<T> Then(const T & t_now) &&
        {
            //written as single return statement for C++11 compatibility
            return (previous_condition)? If(true).Then(std::move(previous_t)) : If(condition_now).Then(t_now);
        }

        constexpr If_Then<T> Then(T && t_now) &&
        {
            //written as single return statement for C++11 compatibility
            return (previous_condition)? If(true).Then(std::move(previous_t)) : If(condition_now).Then(std::move(t_now));
        }

    private:
        constexpr If_Then_Else_If(bool _previous_condition, T && _previous_t, bool _condition_now)
                : previous_condition(_previous_condition), previous_t(std::move(_previous_t)), condition_now(_condition_now)
        {}
        constexpr If_Then_Else_If(bool _previous_condition, const T & _previous_t, bool _condition_now)
                : previous_condition(_previous_condition), previous_t(_previous_t), condition_now(_condition_now)
        {}

#if __cplusplus < 201703L //C++14 and below
        If_Then_Else_If(If_Then_Else_If<T>&& other) = default;
#endif

        bool previous_condition;
        T previous_t;
        bool condition_now;

        friend class If_Then<T>;
    };


    template<typename T>
    class If_Then
    {

    public:

        If_Then & operator=(const If_Then<T> & other) = delete;
        If_Then(const If_Then<T> & other) = delete;

        constexpr If_Then_Else_If<T> ElseIf(bool condition_elseif) &&
        {
            return If_Then_Else_If<T>(condition, std::move(t), condition_elseif );
        }

        constexpr T Else(const T & t_else) &&
        {
            return (condition)?std::move(t):t_else;
        }
        constexpr T Else(T && t_else) &&
        {
            return condition?std::move(t):std::move(t_else);
        }

    private:
        constexpr If_Then(bool _condition, T && _t): condition(_condition), t(std::move(_t)){}
        constexpr If_Then(bool _condition, const T & _t): condition(_condition), t(_t){}

#if __cplusplus < 201703L //C++14 and below
        If_Then(If_Then<T>&& other) =default;
#endif

        bool condition;
        T t;

        friend class If;
        friend class If_Then_Else_If<T>;
    };
}

#endif //FUNCPP_IF_EXPRESSIONS_HPP
