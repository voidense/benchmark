add_executable(bm_json_parsers "benchmark.cc")
target_link_libraries(bm_json_parsers PRIVATE benchmark::benchmark rapidjson nlohmann_json simdjson::simdjson Threads::Threads)
execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/test.json ${CMAKE_CURRENT_BINARY_DIR}/test.json)

