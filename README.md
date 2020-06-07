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
