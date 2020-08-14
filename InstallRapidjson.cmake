if(NOT RAPID_JSON_TAG)
  message(FATAL_ERROR "No RAPID_JSON_TAG specified for rapid_json")
endif()

if(NOT INSTALL_ROOT)
  message(STATUS "INSTALL_ROOT is not defined, using the default")
  set(INSTALL_ROOT ${PROJECT_SOURCE_DIR}/ext CACHE PATH "")
endif()

include(ExternalProject)
ExternalProject_Add(rapidjson_ext
  GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG ${RAPID_JSON_TAG}
  GIT_SUBMODULES ""
  GIT_SHALLOW 1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}
             -DCMAKE_BUILD_TYPE=RELEASE
             -DRAPIDJSON_BUILD_DOC=OFF
             -DRAPIDJSON_BUILD_EXAMPLES=OFF
             -DRAPIDJSON_BUILD_TESTS=OFF
             -DRAPIDJSON_BUILD_THIRDPARTY_GTEST=OFF
             -DRAPIDJSON_BUILD_ASAN=OFF
             -DRAPIDJSON_BUILD_UBSAN=OFF
             -DRAPIDJSON_HAS_STDSTRING=ON
)

