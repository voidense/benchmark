cmake_minimum_required(VERSION 3.16)

option(BUILD_EXT "Whehter a separate upstream build for external dependencies is enabled" OFF)

if(BUILD_EXT)
  project(build-external-dependencies NONE)

  set(INSTALL_ROOT "${PROJECT_SOURCE_DIR}/ext")

  include(GlobalCompilerConfig.cmake)

  # To enable the feature that prevents git submodule updates by setting
  # GIT_SUBMODULES to an empty string
  cmake_policy(SET CMP0097 NEW)


  ## google/benchmark
  set(GOOGLE_BENCHMARK_TAG v1.5.1)
  include(InstallGooglebenchmark.cmake)

  ## boost
  set(BOOST_VERSION 1.73.0)
  # set boost's non-header-only libraries we want to pull in
  set(BOOST_COMPONENTS "system")
  set(BOOST_USE_STATIC_LIBS true)
  include(InstallBoost.cmake)

  ## rapid-json
  set(RAPID_JSON_TAG master)
  include(InstallRapidjson.cmake)

  ## simd-json
  set(SIMD_JSON_TAG v0.4.7)
  include(InstallSimdjson.cmake)

  ## nlohmann-json
  set(NLOHMANN_JSON_TAG v3.9.1)
  include(InstallNlohmannjson.cmake)

else()

  project(benchmark VERSION 0.1 LANGUAGES CXX)

  option(EXT_DEPENDENCIES_IN_TREE "" ON)
  if(EXT_DEPENDENCIES_IN_TREE)
    set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/ext")
  endif()

  ## C++ standard
  include(GlobalCompilerConfig.cmake)

  find_package(RapidJSON REQUIRED)
  find_package(benchmark REQUIRED)
  find_package(Boost REQUIRED)
  find_package(simdjson REQUIRED)
  find_package(nlohmann_json REQUIRED)

  find_package(Threads)

  add_subdirectory(unordered_vs_ordered_map)
  add_subdirectory(shared_vs_unique_ptr)
  add_subdirectory(virtual_vs_non_virtual)
  add_subdirectory(lock_vs_rwlock)
  add_subdirectory(chrono_vs_gettimeofday_or_clockgettime)
  add_subdirectory(string_vs_sstream_in_concat)
  add_subdirectory(any_vs_inheritance)
  add_subdirectory(json_parsers)

endif()
