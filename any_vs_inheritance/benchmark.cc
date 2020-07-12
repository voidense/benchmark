#include <benchmark/benchmark.h>

#include <any>
#include <string>
#include <type_traits>
#include <memory>
#include <vector>

struct Base {
  virtual ~Base() {}
};

template <typename T>
struct Derived : public Base {
  Derived(const T& v) : val(v) {}
  T val;
};


template <typename T>
void BM_inheritance_cast(benchmark::State& state, const T& value) {
  auto d = std::make_unique<Derived<T>>(value);
  std::unique_ptr<Base> b(std::move(d));
  for (auto _ : state) {
    const auto &cast_ret = static_cast<Derived<T> *>(b.get())->val;
    //assert(cast_ret == value);
    benchmark::DoNotOptimize(cast_ret);
  }
}

template <typename T>
void BM_any_cast(benchmark::State& state, const T& value) {
  auto p = std::make_unique<std::any>(value);
  for (auto _ : state) {
    const auto &cast_ret = *std::any_cast<T>(p.get());
    //assert(cast_ret == value);
    benchmark::DoNotOptimize(cast_ret);
  }
}

int i = 5;
std::string s = "123";
struct X {
  X(const int a) : a_(a) {}
  bool operator==(const X &rhs) const {
    return a_ == rhs.a_;
  }
  int a_ = 5; 
};
X x(5);
BENCHMARK_CAPTURE(BM_inheritance_cast, int_value, i);
BENCHMARK_CAPTURE(BM_any_cast, int_value, i);
BENCHMARK_CAPTURE(BM_inheritance_cast, string_value, s);
BENCHMARK_CAPTURE(BM_any_cast, string_value, s);
BENCHMARK_CAPTURE(BM_inheritance_cast, arbitrary_type, x);
BENCHMARK_CAPTURE(BM_any_cast, arbitrary_type, x);


void prepare_vec_base_ptr(std::vector<std::unique_ptr<Base>> &vec, const std::size_t iterations) {
  int si = -5;
  uint64_t ui = 5;
  bool b = true;
  std::string s = "123";
  X x(5);
  for (std::size_t i = 0; i < iterations; ++i) {
    auto d1 = std::make_unique<Derived<int>>(si);
    auto d2 = std::make_unique<Derived<uint64_t>>(ui);
    auto d3 = std::make_unique<Derived<bool>>(b);
    auto d4 = std::make_unique<Derived<std::string>>(s);
    auto d5 = std::make_unique<Derived<X>>(x);
    std::unique_ptr<Base> b1(std::move(d1));
    std::unique_ptr<Base> b2(std::move(d2));
    std::unique_ptr<Base> b3(std::move(d3));
    std::unique_ptr<Base> b4(std::move(d4));
    std::unique_ptr<Base> b5(std::move(d5));
    vec.push_back(std::move(b1));
    vec.push_back(std::move(b2));
    vec.push_back(std::move(b3));
    vec.push_back(std::move(b4));
    vec.push_back(std::move(b5));
  }
}

void prepare_vec_any(std::vector<std::unique_ptr<std::any>> &vec, const std::size_t iterations) {
  int si = -5;
  uint64_t ui = 5;
  bool b = true;
  std::string s = "123";
  X x(5);
  for (std::size_t i = 0; i < iterations; ++i) {
    vec.emplace_back(std::make_unique<std::any>(si));
    vec.emplace_back(std::make_unique<std::any>(ui));
    vec.emplace_back(std::make_unique<std::any>(b));
    vec.emplace_back(std::make_unique<std::any>(s));
    vec.emplace_back(std::make_unique<std::any>(x));
  }
}

void BM_inheritance_cast_branch_predict(
    benchmark::State& state, const std::size_t iterations) {
  std::vector<std::unique_ptr<Base>> vec_base_ptr;
  prepare_vec_base_ptr(vec_base_ptr, iterations);
  for (auto _ : state) {
    for (std::size_t i = 0; i < 5 * iterations; i += 5) {
      auto &cast_ret1 = static_cast<Derived<int> *>(vec_base_ptr[i].get())->val;
      auto &cast_ret2 = static_cast<Derived<uint64_t> *>(vec_base_ptr[i + 1].get())->val;
      auto &cast_ret3 = static_cast<Derived<bool> *>(vec_base_ptr[i + 2].get())->val;
      auto &cast_ret4 = static_cast<Derived<std::string> *>(vec_base_ptr[i + 3].get())->val;
      auto &cast_ret5 = static_cast<Derived<X> *>(vec_base_ptr[i + 4].get())->val;
      //assert(cast_ret1 == -5);
      //assert(cast_ret2 == 5);
      //assert(cast_ret3 == true);
      //assert(cast_ret4 == std::string("123"));
      //assert(cast_ret5 == X(5));
      benchmark::DoNotOptimize(cast_ret1);
      benchmark::DoNotOptimize(cast_ret2);
      benchmark::DoNotOptimize(cast_ret3);
      benchmark::DoNotOptimize(cast_ret4);
      benchmark::DoNotOptimize(cast_ret5);
    }
  }
}

void BM_any_cast_branch_predict(
    benchmark::State& state, const std::size_t iterations) {
  std::vector<std::unique_ptr<std::any>> vec_any_ptr;
  prepare_vec_any(vec_any_ptr, iterations);
  for (auto _ : state) {
    for (std::size_t i = 0; i < 5 * iterations; i += 5) {
      auto &cast_ret1 = *std::any_cast<int>(vec_any_ptr[i].get());
      auto &cast_ret2 = *std::any_cast<uint64_t>(vec_any_ptr[i + 1].get());
      auto &cast_ret3 = *std::any_cast<bool>(vec_any_ptr[i + 2].get());
      auto &cast_ret4 = *std::any_cast<std::string>(vec_any_ptr[i + 3].get());
      auto &cast_ret5 = *std::any_cast<X>(vec_any_ptr[i + 4].get());
      //assert(cast_ret1 == -5);
      //assert(cast_ret2 == 5);
      //assert(cast_ret3 == true);
      //assert(cast_ret4 == std::string("123"));
      //assert(cast_ret5 == X(5));
      benchmark::DoNotOptimize(cast_ret1);
      benchmark::DoNotOptimize(cast_ret2);
      benchmark::DoNotOptimize(cast_ret3);
      benchmark::DoNotOptimize(cast_ret4);
      benchmark::DoNotOptimize(cast_ret5);
    }
  }
}

BENCHMARK_CAPTURE(BM_inheritance_cast_branch_predict, 5_iterations, 5);
BENCHMARK_CAPTURE(BM_any_cast_branch_predict, 5_iterations, 5);
BENCHMARK_CAPTURE(BM_inheritance_cast_branch_predict, 10_iterations, 10);
BENCHMARK_CAPTURE(BM_any_cast_branch_predict, 10_iterations, 10);
BENCHMARK_CAPTURE(BM_inheritance_cast_branch_predict, 100_iterations, 100);
BENCHMARK_CAPTURE(BM_any_cast_branch_predict, 100_iterations, 100);

BENCHMARK_MAIN();

/* sample numbers

Running ./bm_any_vs_inheritance
Run on (8 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 2.15, 2.20, 2.71
--------------------------------------------------------------------------------------------
Benchmark                                                  Time             CPU   Iterations
--------------------------------------------------------------------------------------------
BM_inheritance_cast/int_value                          0.263 ns        0.263 ns   1000000000
BM_any_cast/int_value                                   2.61 ns         2.61 ns    269727691
BM_inheritance_cast/string_value                       0.258 ns        0.258 ns   1000000000
BM_any_cast/string_value                                3.13 ns         3.13 ns    220901721
BM_inheritance_cast/arbitrary_type                     0.259 ns        0.258 ns   1000000000
BM_any_cast/arbitrary_type                              3.72 ns         3.70 ns    194356444
BM_inheritance_cast_branch_predict/5_iterations         6.61 ns         6.55 ns    107084398
BM_any_cast_branch_predict/5_iterations                 94.3 ns         94.1 ns      6813448
BM_inheritance_cast_branch_predict/10_iterations        11.7 ns         11.7 ns     56483955
BM_any_cast_branch_predict/10_iterations                 176 ns          176 ns      3958761
BM_inheritance_cast_branch_predict/100_iterations        119 ns          119 ns      5822077
BM_any_cast_branch_predict/100_iterations               1692 ns         1691 ns       407930

*/
