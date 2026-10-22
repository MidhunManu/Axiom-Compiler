cmake_minimum_required(VERSION 3.20)
project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fixes the CMP0135 warning regarding extraction timestamps
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# -------------------------------------------------------------------
# GoogleTest Integration via FetchContent
# -------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
)

# For Windows/MSVC compatibility: prevents overriding the C runtime libraries
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Don't install gtest along with your project
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# -------------------------------------------------------------------
# Main Application Target
# -------------------------------------------------------------------
add_executable(compiler
    src/main.cpp
    src/lexer.cpp
)
target_include_directories(compiler PRIVATE src)

# -------------------------------------------------------------------
# Test Target
# -------------------------------------------------------------------
add_executable(run_tests
    tests/lexer_test.cpp
    src/lexer.cpp
)

target_include_directories(run_tests PRIVATE src)
target_link_libraries(run_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
