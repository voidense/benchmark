#include <benchmark/benchmark.h>
#include <mutex>
#include <shared_mutex>

std::mutex simple_lock;

// hold a lock on a simple mutex
static void BM_lock_on_simple_mutex(benchmark::State& state) {
  for (auto _ : state) {
    std::scoped_lock hold(simple_lock);
  }
}

std::shared_mutex rw_lock;

// hold a reader's lock on a rw mutex
static void BM_r_lock_on_rw_mutex(benchmark::State& state) {
  for (auto _ : state) {
    std::shared_lock hold(rw_lock);
  }
}

// hold a writer's lock on a rw mutex
static void BM_w_lock_on_rw_mutex(benchmark::State& state) {
  for (auto _ : state) {
    std::unique_lock hold(rw_lock);
  }
}

// Depending on your architecture, you may choose to cut off at your number
// of hardware threads
BENCHMARK(BM_lock_on_simple_mutex)->Threads(2)->Threads(4)->Threads(6)->Threads(8)
  ->Threads(10)->Threads(12)->Threads(14)->Threads(16);
BENCHMARK(BM_r_lock_on_rw_mutex)->Threads(2)->Threads(4)->Threads(6)->Threads(8)
  ->Threads(10)->Threads(12)->Threads(14)->Threads(16);
BENCHMARK(BM_w_lock_on_rw_mutex)->Threads(2)->Threads(4)->Threads(6)->Threads(8)
  ->Threads(10)->Threads(12)->Threads(14)->Threads(16);

BENCHMARK_MAIN();

/* sample numbers

Run on (8 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 2.66, 2.70, 2.48
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
BM_lock_on_simple_mutex/threads:2        49.2 ns         79.6 ns      8202292
BM_lock_on_simple_mutex/threads:4        47.9 ns          151 ns      4419400
BM_lock_on_simple_mutex/threads:6        58.5 ns          284 ns      2472276
BM_lock_on_simple_mutex/threads:8        53.2 ns          353 ns      1894160
BM_lock_on_simple_mutex/threads:10       55.1 ns          392 ns      1557680
BM_lock_on_simple_mutex/threads:12       53.7 ns          403 ns      1793472
BM_lock_on_simple_mutex/threads:14       55.4 ns          414 ns      1670298
BM_lock_on_simple_mutex/threads:16       57.8 ns          399 ns      1936048
BM_r_lock_on_rw_mutex/threads:2           102 ns          164 ns      4226734
BM_r_lock_on_rw_mutex/threads:4           102 ns          314 ns      4784260
BM_r_lock_on_rw_mutex/threads:6           120 ns          585 ns      1197528
BM_r_lock_on_rw_mutex/threads:8           118 ns          797 ns       868112
BM_r_lock_on_rw_mutex/threads:10          133 ns          994 ns       742830
BM_r_lock_on_rw_mutex/threads:12          120 ns          894 ns       707808
BM_r_lock_on_rw_mutex/threads:14          123 ns          958 ns       750498
BM_r_lock_on_rw_mutex/threads:16          112 ns          918 ns       717504
BM_w_lock_on_rw_mutex/threads:2           116 ns          175 ns      3394490
BM_w_lock_on_rw_mutex/threads:4           378 ns         1011 ns       711856
BM_w_lock_on_rw_mutex/threads:6          1546 ns         5669 ns       130068
BM_w_lock_on_rw_mutex/threads:8          1764 ns         7556 ns        71064
BM_w_lock_on_rw_mutex/threads:10         2239 ns        11547 ns        55940
BM_w_lock_on_rw_mutex/threads:12         2871 ns        16693 ns        48744
BM_w_lock_on_rw_mutex/threads:14         2729 ns        16388 ns        39620
BM_w_lock_on_rw_mutex/threads:16         3002 ns        19412 ns        39312

*/

