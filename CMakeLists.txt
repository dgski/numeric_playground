cmake_minimum_required (VERSION 3.15)

project(cmake_test
  LANGUAGES CXX
  VERSION 1.0
)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_subdirectory(BigInt)
add_subdirectory(BigFloat)