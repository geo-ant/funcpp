#ifndef FUNCPP_TYPE_SELECTOR_HPP
#define FUNCPP_TYPE_SELECTOR_HPP

#include <type_traits>

namespace funcpp {
namespace type {

namespace detail {
template <bool Condition, typename T>
struct Then_ {};

template <bool IfCondition, typename T, bool ElifCondition>
struct Elif_ {
  template <typename E>
  using Then = typename std::conditional<IfCondition, Then_<true, T>,
                                         Then_<ElifCondition, E>>::type;
};

template <typename T>
struct Then_<true, T> {
  template <typename E>
  using Else = T;

  template <bool ElifCondition>
  using ElseIf = Elif_<true, T, ElifCondition>;
};
template <typename T>
struct Then_<false, T> {
  template <typename E>
  using Else = E;

  template <bool ElifCondition>
  using ElseIf = Elif_<false, T, ElifCondition>;
};
}  // namespace detail

template <bool Condition>
struct If {
  template <typename T>
  using Then = detail::Then_<Condition, T>;
};

}  // namespace type
}  // namespace funcpp

#endif