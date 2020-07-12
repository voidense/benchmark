
# rapidjson doesn't have a target for its headers, thus we cannot use
# FetchContent and include it as a subdirectory
include(ExternalProject)
ExternalProject_Add(rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
  GIT_TAG master
  GIT_SUBMODULES ""
  GIT_SHALLOW 1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}
             -DRAPIDJSON_BUILD_DOC=OFF
             -DRAPIDJSON_BUILD_EXAMPLES=OFF
             -DRAPIDJSON_BUILD_TESTS=OFF
             -DRAPIDJSON_BUILD_THIRDPARTY_GTEST=OFF
             -DRAPIDJSON_BUILD_ASAN=OFF
             -DRAPIDJSON_BUILD_UBSAN=OFF
             -DRAPIDJSON_HAS_STDSTRING=ON
)
include_directories(${PROJECT_BINARY_DIR}/include)

FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.8.0
  GIT_SUBMODULES ""
  GIT_SHALLOW 1)
FetchContent_GetProperties(nlohmann_json)
if(NOT nlohmann_json_POPULATED)
  FetchContent_Populate(nlohmann_json)
  set(JSON_BuildTests OFF CACHE INTERNAL "")
  add_subdirectory(${nlohmann_json_SOURCE_DIR} ${nlohmann_json_BINARY_DIR})
endif()

FetchContent_Declare(simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG v0.4.1
  GIT_SUBMODULES ""
  GIT_SHALLOW 1)
FetchContent_GetProperties(simdjson)
if(NOT simdjson_POPULATED)
  FetchContent_Populate(simdjson)
  set(SIMDJSON_JUST_LIBRARY ON CACHE INTERNAL "")
  set(SIMDJSON_BUILD_STATIC ON CACHE INTERNAL "")
  add_subdirectory(${simdjson_SOURCE_DIR} ${simdjson_BINARY_DIR})
endif()

add_executable(bm_json_parsers "benchmark.cc")
target_link_libraries(bm_json_parsers PRIVATE benchmark nlohmann_json simdjson)
execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/test.json ${CMAKE_CURRENT_BINARY_DIR}/test.json)


add_dependencies(bm_json_parsers rapidjson)
