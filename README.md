# func++
This, at some point in time, will be a collection of tools to facilitate expressive programming in C++. Written in C++17 and inspired by the ideas of functional programming. It consists of the following subprojects (more to come hopefully).

## A Pipe Syntax for Optionals
Include the header `optional_pipe.hpp` for a pipe (`|`) operator that facilitates applying transformations to values of type `std::optional<T>`. The pipe operator takes an optional as the left hand argument and a callable with signature `F`:`T`&rarr;`U`. It then returns the result of the transformation as a value of type `std::optional<U>`. If the initial optional was empty, then the returned optional is also empty, otherwise it contains the transformed value. The operator can be chained and also accepts callables returning `void`. Those are applied if the given optionals are not empty purely for their side effects. None of the callables may mutate the values inside the optional.

### Example code
```c++
std::optional<std::string> optstr = std::string("Hello Optional World");

auto print_to_console = [](const auto & something)
                        {
                          cout << something<< endl;
                        };

auto square = [](const auto & val){return val*val;};

std::optional<size_t> length = optstr
                               | print_to_console
                               | &std::string::size
                               | print_to_console
                               | square
                               | print_to_console;

cout << "Length is " << length.value_or(0) << endl;
```
The code will output the following:
```
Hello Optional World
20
400
Length is 400
```
### Further Reading
If you are curious, you can read about the implementation on my blog. [Here](https://geo-ant.github.io/blog/2020/optional-pipe-syntax-part-1-fundamentals/) is a good place to start. In the [last part](https://geo-ant.github.io/blog/2020/optional-pipe-syntax-part-3/) of the series I also describe some limitations of the implementation.

## If Expressions to Replace Nested Ternary Operators
Located in the `if_expressions.hpp` header is an If-Then-ElseIf-Else syntax for expressively initializing variables depending on a condition. They can be used to replace nested ternary operators and make initialization code more readable.

### Example Code
Include the header and pull the `If` class in scope by writing `using funcpp::If`. Then you can write

```c++
using funcpp::If;
int x = ... //some value
int sign_of_x = If(x>0).Then(1).ElseIf(x==0).Then(0).Else(-1);
```
This behaves as you would expect, depending on the condition. You can have as many `ElseIf(condition).Then(value)` expressions as you like (or none at all). The most bare-bones usage is just `If(condition).Then(value).Else(alternative_value)`.

If the conditions and values are `constexpr`, then the whole expression can be evauluated at compile time with no runtime penalty. This makes it suitable for e.g. template value arguments.

In case you need to specify the object type (e.g. when initializing a string) you have to be explicit about the type in the first Then clause:
```c++
using funcpp::If;
bool is_french = ... // some value
std::string greeting    = If(is_french).Then(std::string("Bon Jour")).Else("Hello");
//or equivalent:
std::string salutation  = If(is_french).Then<std::string>("Bon Jour").Else("Hello");
```

### Further Reading
You can read about the implementation, more details, and some caveats on my blog [here](https://geo-ant.github.io/blog/2020/if-expressions-for-cpp-part1/).
