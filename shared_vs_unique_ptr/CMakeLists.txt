include_directories("/usr/local/include")
add_executable(bm_shared_vs_unique  "benchmark.cc")
target_link_libraries(bm_shared_vs_unique benchmark::benchmark)

