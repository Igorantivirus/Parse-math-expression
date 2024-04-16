cmake_minimum_required(VERSION 3.12)
project(Parse-math-expression VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS)

set(HEADERS
    MathExpression.hpp
    MathExpressionErrors.hpp
    MathParse.hpp
    ParseUtils.hpp
)

add_library(Parse-math-expression INTERFACE)
target_sources(Parse-math-expression INTERFACE ${HEADERS})

target_include_directories(Parse-math-expression INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(Parse-math-expression INTERFACE Boost::boost)
