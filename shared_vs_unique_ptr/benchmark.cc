#include <benchmark/benchmark.h>
#include <memory>
#include <atomic>
#include <boost/smart_ptr/intrusive_ptr.hpp>

struct X {
  std::atomic<size_t> ref_cnt{0};
};

inline void intrusive_ptr_add_ref(X* x){
  x->ref_cnt.fetch_add(1, std::memory_order_relaxed);
}

inline void intrusive_ptr_release(X* x){
  if (x->ref_cnt.fetch_sub(1, std::memory_order_release) == 1) {
    x->ref_cnt.load(std::memory_order_acquire);
    delete x;
  }
}

static void BM_construct_and_destruct_shared_ptr(benchmark::State& state) {
  for (auto _ : state) {
    auto p = std::make_shared<X>();
    benchmark::DoNotOptimize(p);
  }
}

static void BM_construct_and_destruct_intrusive_ptr(benchmark::State& state) {
  for (auto _ : state) {
    boost::intrusive_ptr p(new X());
    benchmark::DoNotOptimize(p);
  }
}

static void BM_construct_and_destruct_unique_ptr(benchmark::State& state) {
  for (auto _ : state) {
    auto p = std::make_unique<X>();
    benchmark::DoNotOptimize(p);
  }
}

static void BM_construct_and_destruct_raw_ptr(benchmark::State& state) {
  for (auto _ : state) {
    auto p = new X();
    benchmark::DoNotOptimize(p);
    delete p;
  }
}

static void BM_copy_shared_ptr(benchmark::State& state) {
  auto p = std::make_shared<X>();
  for (auto _ : state) {
    // two copies (back and forth)
    auto q = p;
    p = q;
    benchmark::DoNotOptimize(p);
    benchmark::DoNotOptimize(q);
  }
}

static void BM_copy_intrusive_ptr(benchmark::State& state) {
  boost::intrusive_ptr<X> p(new X());
  for (auto _ : state) {
    // two copies (back and forth)
    auto q = p;
    p = q;
    benchmark::DoNotOptimize(p);
    benchmark::DoNotOptimize(q);
  }
}

static void BM_move_unique_ptr(benchmark::State& state) {
  auto p = std::make_unique<X>();
  for (auto _ : state) {
    // two moves (back and forth)
    auto q = std::move(p);
    p = std::move(q);
    benchmark::DoNotOptimize(p);
    benchmark::DoNotOptimize(q);
  }
}

static void BM_copy_raw_ptr(benchmark::State& state) {
  auto p = new X();
  for (auto _ : state) {
    // two copies (back and forth)
    auto q = p;
    p = q;
    benchmark::DoNotOptimize(p);
    benchmark::DoNotOptimize(q);

  }
  delete p;
}

BENCHMARK(BM_construct_and_destruct_shared_ptr);

BENCHMARK(BM_construct_and_destruct_intrusive_ptr);

BENCHMARK(BM_construct_and_destruct_unique_ptr);

BENCHMARK(BM_construct_and_destruct_raw_ptr);


BENCHMARK(BM_copy_shared_ptr);

BENCHMARK(BM_copy_intrusive_ptr);

BENCHMARK(BM_move_unique_ptr);

BENCHMARK(BM_copy_raw_ptr);


BENCHMARK_MAIN();

/* sample numbers

Run on (8 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 2.25, 2.31, 2.34
-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_construct_and_destruct_raw_ptr          68.5 ns         68.5 ns     10028079
BM_construct_and_destruct_shared_ptr       81.3 ns         81.2 ns      8493084
BM_construct_and_destruct_unique_ptr       70.3 ns         70.2 ns      9864991
BM_copy_shared_ptr                         22.3 ns         22.2 ns     30460740
BM_move_unique_ptr                         1.38 ns         1.37 ns    514433535
BM_copy_raw_ptr                           0.272 ns        0.271 ns   1000000000

*/
