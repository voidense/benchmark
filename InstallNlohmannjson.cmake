if(NOT NLOHMANN_JSON_TAG)
  message(FATAL_ERROR "No NLOHMANN_JSON_TAGspecified for nlohmann/json")
endif()

if(NOT INSTALL_ROOT)
  message(STATUS "INSTALL_ROOT is not defined, using default")
  set(INSTALL_ROOT ${PROJECT_SOURCE_DIR}/ext CACHE PATH "")
endif()

include(ExternalProject)
ExternalProject_Add(nlohmann_json_ext
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG ${NLOHMANN_JSON_TAG}
  GIT_SUBMODULES ""
  GIT_SHALLOW 1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}
             -DCMAKE_BUILD_TYPE=RELEASE
             -DJSON_BuildTests=OFF
             -DJSON_Install=ON
)

