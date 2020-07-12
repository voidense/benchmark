cmake_minimum_required(VERSION 3.16)

project(benchmark VERSION 0.1 LANGUAGES CXX)

# To enable the feature that prevents git submodule updates by setting
# GIT_SUBMODULES to an empty string
cmake_policy(SET CMP0097 NEW)


## C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.5.1
  GIT_SHALLOW 1
  GIT_SUBMODULES ""
)
FetchContent_GetProperties(googlebenchmark)
if(NOT googlebenchmark_POPULATED)
  FetchContent_Populate(googlebenchmark)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
  add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR})
endif()

set(BOOST_COMPONENTS thread)
set(BOOST_USE_STATIC_LIBS 1)
set(BOOST_VERSION 1.73.0)
include(InstallBoost.cmake)

if(NOT ${APPLE})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
endif()

add_compile_options(-g)
add_compile_options(-Wall)
add_compile_options(-O3)

add_subdirectory(unordered_vs_ordered_map)
add_subdirectory(shared_vs_unique_ptr)
add_subdirectory(virtual_vs_non_virtual)
add_subdirectory(lock_vs_rwlock)
add_subdirectory(chrono_vs_gettimeofday_or_clockgettime)
add_subdirectory(string_vs_sstream_in_concat)
add_subdirectory(any_vs_inheritance)
add_subdirectory(json_parsers)
