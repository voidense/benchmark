if(NOT SIMD_JSON_TAG)
  message(FATAL_ERROR "No SIMD_JSON_TAG specified for simdjson")
endif()

if(NOT INSTALL_ROOT)
  message(STATUS "INSTALL_ROOT is not defined, using the default")
  set(INSTALL_ROOT ${PROJECT_SOURCE_DIR}/ext CACHE PATH "")
endif()

include(ExternalProject)
ExternalProject_Add(simdjson_ext
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG ${SIMD_JSON_TAG}
  GIT_SUBMODULES ""
  GIT_SHALLOW 1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}
             -DCMAKE_BUILD_TYPE=RELEASE
             -DSIMDJSON_JUST_LIBRARY=ON
             -DSIMDJSON_BUILD_STATIC=ON
)

