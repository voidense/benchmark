add_executable(bm_std_function_vs_virtual_function "benchmark.cc")
target_link_libraries(bm_std_function_vs_virtual_function PRIVATE benchmark::benchmark Threads::Threads)

