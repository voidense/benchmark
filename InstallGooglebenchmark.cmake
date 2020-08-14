if(NOT GOOGLE_BENCHMARK_TAG)
  message(FATAL_ERROR "No GOOLGE_BENCHMARK_TAG specified for google/benchmark")
endif()

if(NOT INSTALL_ROOT)
  message(STATUS "INSTALL_ROOT is not defined, using default")
  set(INSTALL_ROOT ${PROJECT_SOURCE_DIR}/ext CACHE PATH "")
endif()

include(ExternalProject)
ExternalProject_Add(googlebenchmark_ext
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG ${GOOGLE_BENCHMARK_TAG}
  GIT_SHALLOW 1
  GIT_SUBMODULES ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}
             -DCMAKE_BUILD_TYPE=RELEASE
             -DBENCHMARK_ENABLE_TESTING=OFF
             -DBENCHMARK_ENABLE_INSTALL=ON
)

