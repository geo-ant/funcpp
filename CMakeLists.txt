cmake_minimum_required(VERSION 3.8)
project(funcpp)

set(CMAKE_CXX_STANDARD 11)

# see here https://docs.conan.io/en/latest/integrations/ide/clion.html
# and here for conan integration for https://github.com/jw3/example-catch2-conan
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/build)
find_package(Catch2 2 REQUIRED)
include_directories(include/ ${CONAN_INCLUDE_DIRS_CATCH2})
enable_testing()

add_library(funcpp INTERFACE)
target_include_directories(funcpp INTERFACE include/)

add_executable(test_if_expressions test/test_if_expr.cpp test/test_main.cpp)
set_property(TARGET test_if_expressions PROPERTY CXX_STANDARD 17)
target_link_libraries(test_if_expressions PRIVATE funcpp)
add_test(NAME test_if_expressions COMMAND test_if_expressions)
target_link_libraries(test_if_expressions PRIVATE Catch2::Catch2)

add_compile_options("-Wall" "-Werror" "-Wextra")

