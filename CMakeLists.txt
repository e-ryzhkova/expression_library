cmake_minimum_required(VERSION 3.10)

project(expression_library VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DOCS "Build Doxygen documentation" OFF)

include(GNUInstallDirs)

set(LIBRARY_SOURCES
    src/core/expression_api.c
    src/core/error.c
    src/core/memory.c
    src/core/benchmark.c
    src/core/testing.c
    src/core/visualization.c

    src/conversion/shunting_yard.c
    src/conversion/postfix_eval.c
    src/conversion/syntax_check.c
    src/conversion/infix_eval.c

    src/ast/ast_build.c
    src/ast/ast_eval.c
    src/ast/ast_prefix.c
    src/ast/ast_constant_fold.c

    src/symbolic/simplify.c
    src/symbolic/substitute.c
    src/symbolic/differentiate.c
    src/symbolic/equal.c
)

add_library(expression STATIC ${LIBRARY_SOURCES})

target_include_directories(expression
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(expression PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(expression PUBLIC m)

# Examples
if(BUILD_EXAMPLES)
    foreach(EX
        basic_calc
        ast_walk
        symbolic_derivative
        visualization_demo
        benchmark_demo
    )
        add_executable(example_${EX} examples/${EX}.c)
        target_link_libraries(example_${EX} PRIVATE expression)
    endforeach()
endif()

# Benchmarks
if(BUILD_BENCHMARKS)
    add_executable(time_benchmark benchmarks/time_benchmark.c)
    target_link_libraries(time_benchmark PRIVATE expression)

    add_executable(memory_benchmark benchmarks/memory_benchmark.c)
    target_link_libraries(memory_benchmark PRIVATE expression)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()

    foreach(T
        conversion
        ast
        symbolic
        api
        property
    )
        add_executable(test_${T} tests/test_${T}.c)
        target_link_libraries(test_${T} PRIVATE expression)
        add_test(NAME test_${T} COMMAND test_${T})
    endforeach()
endif()

install(TARGETS expression
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
