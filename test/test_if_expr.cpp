//
// Created by georgios on 24.07.20.
//

#include "catch2/catch.hpp"
#include "funcpp/if_expressions.hpp"
#include <thread>
#include <utility>

using funcpp::If;

template<typename T>
T get_uniform_random_number(T minimum, T maximum);

bool get_random_bool();

// helper class that lets me find out whether the moves
// are correctly executed
class helper {
public:
    explicit helper(int val)
            : value(val)
    {}
/*
    helper(const helper& other)
            : value(other.value) ,
              copy_count(other.copy_count+1),
              move_count(other.move_count)
    {}
*/
    helper(const helper & other) = delete;


    helper(helper&& other)
            : value(other.value) ,
              copy_count(other.copy_count),
              move_count(other.move_count+1)
    {}

    const helper& operator=(const helper & ) = delete;
    const helper& operator=(helper &&) = delete;

    int value;
    const int copy_count = 0;
    const int move_count = 0;
};
TEST_CASE("If-Else Expressions produce correct results") {

    SECTION("Constexpr conditions") {
        constexpr int one = If(true).Then(1).Else(0);
        REQUIRE(one == 1);

        constexpr int zero = If(false).Then(1).Else(0);
        REQUIRE(zero == 0);
    }

    SECTION("Runtime conditions") {
        bool condition = get_random_bool();
        int value1 = If(condition).Then(1).Else(0);
        int expected1 = condition?1:0;
        REQUIRE(value1 == expected1);

        int value2 = If(!condition).Then(1).Else(0);
        int expected2 =  (!condition)?1:0;
        REQUIRE(value2 == expected2);
    }
}

TEST_CASE("If-Then-ElseIf-...-Else Produces correct results") {
    SECTION("Value of first true condition is returned") {
        int value = If(true).Then(0).ElseIf(true).Then(1).ElseIf(false).Then(2).Else(3);
        REQUIRE (value == 0);

        value = If(false).Then(0).ElseIf(true).Then(1).ElseIf(true).Then(2).Else(3);
        REQUIRE(value == 1);

        value = If(false).Then(0).ElseIf(false).Then(1).ElseIf(true).Then(2).Else(3);
        REQUIRE(value == 2);
    }

    SECTION("If no conditions are true, the Else value is returned") {
        int value = If(false).Then(0).ElseIf(false).Then(1).ElseIf(false).Then(2).Else(3);
        REQUIRE(value == 3);
    }
}

TEST_CASE("Objects are moved wherever possible"){

    SECTION("Move out of If") {
        helper hlp1 = If(true).Then(helper(0)).Else(helper(1));
        REQUIRE(hlp1.copy_count == 0);
    }

    SECTION("Move out of Else"){
        helper hlp2 = If(false).Then(helper(0)).Else(helper(1));
        REQUIRE(hlp2.copy_count == 0);
    }

    SECTION("Move out of ElseIf") {
        helper hlp3 = If(false).Then(helper(0)).ElseIf(true).Then(helper(1)).Else(helper(2));
        REQUIRE(hlp3.copy_count == 0);
    }

    SECTION("Move out of Else after ElseIf") {
        helper hlp4 = If(false).Then(helper(0)).ElseIf(false).Then(helper(1)).Else(helper(2));
        REQUIRE(hlp4.copy_count == 0);
    }

    SECTION("Move out of Else after ElseIf") {
        helper hlp4 = If(true).Then(helper(0)).ElseIf(false).Then(helper(1)).Else(helper(2));
        REQUIRE(hlp4.copy_count == 0);
    }
}



template<typename T>
T get_uniform_random_number(T minimum, T maximum)
{
using thread_id_hash = std::hash<std::thread::id>;
static thread_local std::random_device rd;
static thread_local T seed = rd() + std::chrono::system_clock::now().time_since_epoch().count() + thread_id_hash()(std::this_thread::get_id());
static thread_local std::mt19937 generator(seed);
std::uniform_int_distribution<T> distribution(minimum,maximum);
return distribution(generator);
}

bool get_random_bool() {
    return get_uniform_random_number<int>(0,1)==1;
}