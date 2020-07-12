#include <benchmark/benchmark.h>
#include <sys/time.h>
#include <time.h>
#include <chrono>

// get result in micro seconds

static void BM_chrono_sysclock_time_since_epoch(benchmark::State& state) {
  for (auto _ : state) {
    auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
    auto microSecs = std::chrono::duration_cast<
        std::chrono::microseconds>(timeSinceEpoch ).count();
    benchmark::DoNotOptimize(microSecs);
  }
}

static void BM_chrono_steadyclock_time_since_epoch(benchmark::State& state) {
  for (auto _ : state) {
    auto timeSinceEpoch = std::chrono::steady_clock::now().time_since_epoch();
    auto microSecs = std::chrono::duration_cast<
        std::chrono::microseconds>(timeSinceEpoch ).count();
    benchmark::DoNotOptimize(microSecs);
  }
}

static void BM_gettimeofday_time_since_epoch(benchmark::State& state) {
  for (auto _ : state) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    auto microSecs = tv.tv_usec + 1000 * 1000 * tv.tv_sec;
    benchmark::DoNotOptimize(microSecs);
  }
}

static void BM_clockgettime_time_since_epoch(benchmark::State& state) {
  for (auto _ : state) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    auto microSecs = ts.tv_nsec / 1000 + 1000 * 1000 * ts.tv_sec;
    benchmark::DoNotOptimize(microSecs);
  }
}

BENCHMARK(BM_chrono_sysclock_time_since_epoch);
BENCHMARK(BM_chrono_steadyclock_time_since_epoch);
BENCHMARK(BM_gettimeofday_time_since_epoch);
BENCHMARK(BM_clockgettime_time_since_epoch);

BENCHMARK_MAIN();

/* sample numbers

Run on (8 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 2.33, 2.31, 2.37
---------------------------------------------------------------------------------
Benchmark                                       Time             CPU   Iterations
---------------------------------------------------------------------------------
BM_chrono_sysclock_time_since_epoch          29.2 ns         29.1 ns     24062177
BM_chrono_steadyclock_time_since_epoch       35.7 ns         35.7 ns     19524604
BM_gettimeofday_time_since_epoch             23.3 ns         23.3 ns     29804483
BM_clockgettime_time_since_epoch             28.3 ns         28.2 ns     24574079

*/

