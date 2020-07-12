add_executable(bm_virtual_vs_non_virtual  "benchmark.cc")
target_include_directories(bm_virtual_vs_non_virtual PRIVATE /usr/local/include)
target_link_libraries(bm_virtual_vs_non_virtual /usr/local/lib/libbenchmark.a)

