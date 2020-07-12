#include <benchmark/benchmark.h>

struct NonDerived {
  int foo() {return 1;}
};

struct Base {
  virtual ~Base() {}
  virtual int foo() = 0;
};

struct Derived : public Base {
  int foo() override {return 1;}
};


static void BM_call_from_base_ptr(benchmark::State& state) {
  Derived *d = new Derived();
  Base *b = static_cast<Base *>(d);
  for (auto _ : state) {
    benchmark::DoNotOptimize(b->foo());
  }
  delete d;
}

static void BM_call_from_derived_ptr(benchmark::State& state) {
  Derived *d = new Derived();
  for (auto _ : state) {
    benchmark::DoNotOptimize(d->foo());
  }
  delete d;
}

static void BM_call_from_non_derived(benchmark::State& state) {
  NonDerived *nd = new NonDerived();
  for (auto _ : state) {
    benchmark::DoNotOptimize(nd->foo());
  }
  delete nd;
}


BENCHMARK(BM_call_from_non_derived);
BENCHMARK(BM_call_from_base_ptr);
BENCHMARK(BM_call_from_derived_ptr);


BENCHMARK_MAIN();

/* sample numbers

Run on (8 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 2.36, 2.33, 2.36
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
BM_call_from_non_derived      0.277 ns        0.276 ns   1000000000
BM_call_from_base_ptr          1.37 ns         1.37 ns    497572557
BM_call_from_derived_ptr       1.37 ns         1.37 ns    486959909

*/
