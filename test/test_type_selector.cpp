// we don't need catch here because we can test everything with static assertions
#include <type_traits>
#include "funcpp/type_selector.hpp"

using namespace funcpp::type;


// a random use case
constexpr int i = 2;
using LastType = If<i < 2>::Then<int>::ElseIf<(i<3)>::Then<float>::Else<bool>;
static_assert(std::is_same<LastType,float>::value);

// structured testing
using A1 = If<true>::Then<float>::Else<bool>;
static_assert(std::is_same<A1,float>::value);
using A2 = If<false>::Then<float>::Else<bool>;
static_assert(std::is_same<A2,bool>::value);

using B1 = If<true>::Then<float>::ElseIf<false>::Then<bool>::Else<int>;
static_assert(std::is_same<B1,float>::value);
using B2 = If<true>::Then<float>::ElseIf<true>::Then<bool>::Else<int>;
static_assert(std::is_same<B2,float>::value);
using B3 = If<false>::Then<float>::ElseIf<true>::Then<bool>::Else<int>;
static_assert(std::is_same<B3,bool>::value);
using B4 = If<false>::Then<float>::ElseIf<false>::Then<bool>::Else<int>;
static_assert(std::is_same<B4,int>::value);

struct dummy{};

using C1 = If<true>::Then<int>::ElseIf<false>::Then<float>::ElseIf<false>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C1,int>::value);
using C2 = If<false>::Then<int>::ElseIf<true>::Then<float>::ElseIf<false>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C2,float>::value);
using C3 = If<false>::Then<int>::ElseIf<false>::Then<float>::ElseIf<true>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C3,bool>::value);
using C4 = If<true>::Then<int>::ElseIf<true>::Then<float>::ElseIf<false>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C4,int>::value);
using C5 = If<true>::Then<int>::ElseIf<true>::Then<float>::ElseIf<true>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C5,int>::value);
using C6 = If<false>::Then<int>::ElseIf<true>::Then<float>::ElseIf<true>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C6,float>::value);
using C7 = If<false>::Then<int>::ElseIf<false>::Then<float>::ElseIf<false>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C7,dummy>::value);
using C8 = If<true>::Then<int>::ElseIf<true>::Then<float>::ElseIf<true>::Then<bool>::Else<dummy>;
static_assert(std::is_same<C8,int>::value);

// this is only so that CTest understands that this test passes
int main() {
  return 0;
}