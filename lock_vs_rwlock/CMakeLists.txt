add_executable(bm_lock_vs_rwlock  "benchmark.cc")
target_link_libraries(bm_lock_vs_rwlock benchmark::benchmark)

