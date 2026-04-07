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

include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_HEADERS
    include/expression.h
    include/expression_types.h
    include/expression_core.h
    include/conversion.h
    include/ast.h
    include/symbolic.h
)

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

add_library(expression STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

target_include_directories(expression
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(MSVC)
    target_compile_options(expression PRIVATE /W4)
else()
    target_compile_options(expression PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(expression PUBLIC m)

if(BUILD_EXAMPLES)
    add_executable(example_basic_calc examples/basic_calc.c)
    target_link_libraries(example_basic_calc PRIVATE expression)

    add_executable(example_ast_walk examples/ast_walk.c)
    target_link_libraries(example_ast_walk PRIVATE expression)

    add_executable(example_symbolic_derivative examples/symbolic_derivative.c)
    target_link_libraries(example_symbolic_derivative PRIVATE expression)

    add_executable(example_visualization_demo examples/visualization_demo.c)
    target_link_libraries(example_visualization_demo PRIVATE expression)

    add_executable(example_benchmark_demo examples/benchmark_demo.c)
    target_link_libraries(example_benchmark_demo PRIVATE expression)
endif()

if(BUILD_BENCHMARKS)
    add_executable(time_benchmark benchmarks/time_benchmark.c)
    target_link_libraries(time_benchmark PRIVATE expression)

    add_executable(memory_benchmark benchmarks/memory_benchmark.c)
    target_link_libraries(memory_benchmark PRIVATE expression)
endif()

if(BUILD_TESTS)
    enable_testing()

    add_executable(test_conversion tests/test_conversion.c)
    target_link_libraries(test_conversion PRIVATE expression)
    add_test(NAME test_conversion COMMAND test_conversion)

    add_executable(test_ast tests/test_ast.c)
    target_link_libraries(test_ast PRIVATE expression)
    add_test(NAME test_ast COMMAND test_ast)

    add_executable(test_symbolic tests/test_symbolic.c)
    target_link_libraries(test_symbolic PRIVATE expression)
    add_test(NAME test_symbolic COMMAND test_symbolic)

    add_executable(test_api tests/test_api.c)
    target_link_libraries(test_api PRIVATE expression)
    add_test(NAME test_api COMMAND test_api)

    add_executable(test_property tests/test_property.c)
    target_link_libraries(test_property PRIVATE expression)
    add_test(NAME test_property COMMAND test_property)
endif()

if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    else()
        message(WARNING "Doxygen not found, documentation target will not be available.")
    endif()
endif()

install(TARGETS expression
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES
    include/expression.h
    include/expression_types.h
    include/conversion.h
    include/ast.h
    include/symbolic.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

