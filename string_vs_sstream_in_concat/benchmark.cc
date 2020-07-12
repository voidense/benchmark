#include <benchmark/benchmark.h>
#include <string>
#include <sstream>

#define BM_COMPARISION_GROUP(NUM_ITER) \
\
static void BM_string_construct_concat_destruct_ ## NUM_ITER ## _concatenations(benchmark::State& state) { \
  for (auto _ : state) { \
    std::string s; \
    for (std::size_t i = 0; i < NUM_ITER ; ++i) { \
      s += "123"; \
    } \
    benchmark::DoNotOptimize(s); \
  } \
} \
\
static void BM_string_construct_concat_with_reserve_destruct_ ## NUM_ITER ## _concatenations(benchmark::State& state) { \
  for (auto _ : state) { \
    std::string s; \
    s.reserve(NUM_ITER * 3); \
    for (std::size_t i = 0; i < NUM_ITER ; ++i) { \
      s += "123"; \
    } \
    benchmark::DoNotOptimize(s); \
  } \
} \
\
static void BM_sstream_construct_concat_destruct_ ## NUM_ITER ## _concatenations(benchmark::State& state) { \
  for (auto _ : state) { \
    std::ostringstream oss; \
    for (std::size_t i = 0; i < NUM_ITER ; ++i) { \
      oss << "123"; \
    } \
    std::string out = oss.str(); \
    benchmark::DoNotOptimize(out); \
  } \
} \
\
BENCHMARK(BM_string_construct_concat_destruct_ ## NUM_ITER ## _concatenations); \
BENCHMARK(BM_string_construct_concat_with_reserve_destruct_ ## NUM_ITER ## _concatenations); \
BENCHMARK(BM_sstream_construct_concat_destruct_ ## NUM_ITER ## _concatenations); \

BM_COMPARISION_GROUP(1)
BM_COMPARISION_GROUP(2)
BM_COMPARISION_GROUP(4)
BM_COMPARISION_GROUP(8)
BM_COMPARISION_GROUP(16)
BM_COMPARISION_GROUP(32)
BM_COMPARISION_GROUP(64)
BM_COMPARISION_GROUP(128)

BENCHMARK_MAIN();

/* sample numbers

Run on (8 X 2200 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 1.89, 1.97, 2.06
***WARNING*** Library was built as DEBUG. Timings may be affected.
--------------------------------------------------------------------------------------------------------------
Benchmark                                                                    Time             CPU   Iterations
--------------------------------------------------------------------------------------------------------------
BM_string_construct_concat_destruct_1_concatenations                      13.0 ns         12.9 ns     54354156
BM_string_construct_concat_with_reserve_destruct_1_concatenations         18.0 ns         18.0 ns     37544383
BM_sstream_construct_concat_destruct_1_concatenations                      192 ns          192 ns      3735784
BM_string_construct_concat_destruct_2_concatenations                      26.4 ns         26.3 ns     26856040
BM_string_construct_concat_with_reserve_destruct_2_concatenations         30.9 ns         30.9 ns     23165078
BM_sstream_construct_concat_destruct_2_concatenations                      234 ns          234 ns      3025169
BM_string_construct_concat_destruct_4_concatenations                      49.6 ns         49.6 ns     13253058
BM_string_construct_concat_with_reserve_destruct_4_concatenations         51.9 ns         51.9 ns     12073753
BM_sstream_construct_concat_destruct_4_concatenations                      300 ns          300 ns      2242318
BM_string_construct_concat_destruct_8_concatenations                       165 ns          165 ns      4207742
BM_string_construct_concat_with_reserve_destruct_8_concatenations          171 ns          171 ns      3939999
BM_sstream_construct_concat_destruct_8_concatenations                      736 ns          736 ns       923532
BM_string_construct_concat_destruct_16_concatenations                      417 ns          417 ns      1685147
BM_string_construct_concat_with_reserve_destruct_16_concatenations         255 ns          255 ns      2710080
BM_sstream_construct_concat_destruct_16_concatenations                    1206 ns         1206 ns       483816
BM_string_construct_concat_destruct_32_concatenations                      837 ns          837 ns       837240
BM_string_construct_concat_with_reserve_destruct_32_concatenations         441 ns          441 ns      1542288
BM_sstream_construct_concat_destruct_32_concatenations                    2212 ns         2211 ns       312351
BM_string_construct_concat_destruct_64_concatenations                     1402 ns         1402 ns       496905
BM_string_construct_concat_with_reserve_destruct_64_concatenations         853 ns          853 ns       742516
BM_sstream_construct_concat_destruct_64_concatenations                    3892 ns         3891 ns       183931
BM_string_construct_concat_destruct_128_concatenations                    2250 ns         2250 ns       293667
BM_string_construct_concat_with_reserve_destruct_128_concatenations       1692 ns         1692 ns       406230
BM_sstream_construct_concat_destruct_128_concatenations                   6682 ns         6680 ns       102090


*/
