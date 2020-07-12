#include <benchmark/benchmark.h>
#include <unordered_map>
#include <map>
#include <string>
#define MAP_DATA_5  \
  { \
    {"111", 1}, {"222", 1}, {"333", 1}, \
    {"444", 1}, {"555", 1}, \
  };

#define RANDOM_KEY_SEQ_5 \
  { \
    "222", "555", "111", "444", "333", \
  }

#define MAP_DATA_20  \
  { \
    {"111", 1}, {"222", 1}, {"333", 1}, \
    {"444", 1}, {"555", 1}, {"666", 1}, \
    {"777", 1}, {"888", 1}, {"999", 1}, \
    {"101010", 1}, {"111111", 1}, {"121212", 1}, \
    {"131313", 1}, {"141414", 1}, {"151515", 1}, \
    {"161616", 1}, {"171717", 1}, {"181818", 1}, \
    {"191919", 1}, {"202020", 1} \
  };

#define RANDOM_KEY_SEQ_20 \
  { \
    "181818", "101010", "121212", "222", "202020", "999", "171717", "666", \
    "111111", "161616", "888", "151515", "555", "191919", "131313", "111", \
    "444", "141414", "333",  "777", \
  }

#define MAP_DATA_40  \
  { \
    {"111", 1}, {"222", 1}, {"333", 1}, \
    {"444", 1}, {"555", 1}, {"666", 1}, \
    {"777", 1}, {"888", 1}, {"999", 1}, \
    {"101010", 1}, {"111111", 1}, {"121212", 1}, \
    {"131313", 1}, {"141414", 1}, {"151515", 1}, \
    {"161616", 1}, {"171717", 1}, {"181818", 1}, \
    {"191919", 1}, {"202020", 1}, {"212121", 1}, \
    {"222222", 1}, {"232323", 1}, {"242424", 1}, \
    {"252525", 1}, {"262626", 1}, {"272727", 1}, \
    {"282828", 1}, {"292929", 1}, {"303030", 1}, \
    {"313131", 1}, {"323232", 1}, {"333333", 1}, \
    {"343434", 1}, {"353535", 1}, {"363636", 1}, \
    {"373737", 1}, {"383838", 1}, {"393939", 1}, \
    {"404040", 1}, \
  };

#define RANDOM_KEY_SEQ_40 \
  { \
    "272727", "323232", "111", "191919", "444", \
    "333", "101010", "333333", "111111", "282828", \
    "292929", "313131", "171717", "252525", "242424", \
    "404040", "141414", "303030", "131313", "222", \
    "212121", "777", "393939", "181818", "262626", \
    "666", "888", "151515", "343434", "555", \
    "161616", "232323", "222222", "121212", \
    "202020", "353535", "373737", "383838", "363636", \
    "999", \
  }


#define BM_unordered_all_hit_random_access(num_entries) \
static void BM_unordered_all_hit_random_access_ ## num_entries ## _entries(benchmark::State& state) { \
  std::unordered_map<std::string, int> m = MAP_DATA_ ## num_entries; \
  for (auto _ : state) { \
    for (const auto &key : RANDOM_KEY_SEQ_ ## num_entries) { \
      const auto &val = m.at(key); \
      benchmark::DoNotOptimize(val); \
    } \
  } \
} \
BENCHMARK(BM_unordered_all_hit_random_access_ ## num_entries ## _entries);

#define BM_unordered_one_hit_random_access(num_entries) \
static void BM_unordered_one_hit_random_access_ ## num_entries ## _entries(benchmark::State& state) { \
  std::unordered_map<std::string, int> m = MAP_DATA_ ## num_entries; \
  for (auto _ : state) { \
    const auto &val = m.at("333"); \
    benchmark::DoNotOptimize(val); \
  } \
} \
BENCHMARK(BM_unordered_one_hit_random_access_ ## num_entries ## _entries);

#define BM_ordered_all_hit_random_access(num_entries) \
static void BM_ordered_all_hit_random_access_ ## num_entries ## _entries(benchmark::State& state) { \
  std::map<std::string, int> m = MAP_DATA_ ## num_entries; \
  for (auto _ : state) { \
    for (const auto &key : RANDOM_KEY_SEQ_ ## num_entries) { \
      const auto &val = m.at(key); \
      benchmark::DoNotOptimize(val); \
    } \
  } \
} \
BENCHMARK(BM_ordered_all_hit_random_access_ ## num_entries ## _entries);

#define BM_ordered_one_hit_random_access(num_entries) \
static void BM_ordered_one_hit_random_access_ ## num_entries ## _entries(benchmark::State& state) { \
  std::map<std::string, int> m = MAP_DATA_ ## num_entries; \
  for (auto _ : state) { \
    const auto &val = m.at("333"); \
    benchmark::DoNotOptimize(val); \
  } \
} \
BENCHMARK(BM_ordered_one_hit_random_access_ ## num_entries ## _entries);

// one hit latency could be biased because in the case of an ordered map, it
// could be lucky/unlucky, depending on how many nodes to walk down the tree
// to find the hit
BM_unordered_one_hit_random_access(5);
BM_ordered_one_hit_random_access(5);
BM_unordered_one_hit_random_access(20);
BM_ordered_one_hit_random_access(20);
BM_unordered_one_hit_random_access(40);
BM_ordered_one_hit_random_access(40);

// the time used to find the hit is amortized through a random key sequence
BM_unordered_all_hit_random_access(5);
BM_ordered_all_hit_random_access(5);
BM_unordered_all_hit_random_access(20);
BM_ordered_all_hit_random_access(20);
BM_unordered_all_hit_random_access(40);
BM_ordered_all_hit_random_access(40);

BENCHMARK_MAIN();

/* sample numbers

Run on (8 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 2.20, 2.30, 2.34
----------------------------------------------------------------------------------------
Benchmark                                              Time             CPU   Iterations
----------------------------------------------------------------------------------------
BM_unordered_one_hit_random_access_5_entries        21.4 ns         21.1 ns     33447373
BM_ordered_one_hit_random_access_5_entries          24.7 ns         24.7 ns     27186472
BM_unordered_one_hit_random_access_20_entries       20.7 ns         20.6 ns     34028156
BM_ordered_one_hit_random_access_20_entries         31.4 ns         31.3 ns     22101820
BM_unordered_one_hit_random_access_40_entries       20.6 ns         20.6 ns     33522496
BM_ordered_one_hit_random_access_40_entries         26.9 ns         26.8 ns     26244357
BM_unordered_all_hit_random_access_5_entries         135 ns          134 ns      5145545
BM_ordered_all_hit_random_access_5_entries           138 ns          138 ns      5069268
BM_unordered_all_hit_random_access_20_entries        681 ns          681 ns      1012834
BM_ordered_all_hit_random_access_20_entries          837 ns          837 ns       826651
BM_unordered_all_hit_random_access_40_entries       1490 ns         1489 ns       467452
BM_ordered_all_hit_random_access_40_entries         2266 ns         2265 ns       305165

*/
