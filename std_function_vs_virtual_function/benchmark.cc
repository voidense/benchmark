#include <benchmark/benchmark.h>

#include <functional>
#include <boost/function.hpp>

struct base {
  virtual ~base() {}
  virtual size_t run() = 0;
};

constexpr size_t max = 1000000;

struct derived1 : public base {
  size_t mod_ = 0;
  derived1(size_t mod) : mod_(mod) {}
  size_t run() override {
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod_ == 1) {
        ret += i;
      }
    }
    return ret;
  }
};
struct derived2 : public base {
  size_t mod_ = 0;
  derived2(size_t mod) : mod_(mod) {}
  size_t run() override {
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod_ != 1) {
        ret += i;
      }
    }
    return ret;
  }
};


static void BM_virtual(benchmark::State& state) {
  auto p1 = static_cast<base *>(new derived1(3));
  auto p2 = static_cast<base *>(new derived2(5));
  static_assert(std::is_same_v<decltype(p1), decltype(p2)>);
  for (auto _ : state) {
    auto ret = p1->run() + p2->run();
    benchmark::DoNotOptimize(ret);
  }
}

static void BM_std_function(benchmark::State& state) {
  std::function<size_t(size_t)> f1 = [](size_t mod)->size_t{
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod == 1) {
        ret += i;
      }
    }
    return ret;
  };
  std::function<size_t(size_t)> f2 = [](size_t mod)->size_t{
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod != 1) {
        ret += i;
      }
    }
    return ret;
  };
  static_assert(std::is_same_v<decltype(f1), decltype(f2)>);
  for (auto _ : state) {
    auto ret = f1(3) + f2(5);
    benchmark::DoNotOptimize(ret);
  }
}

static void BM_boost_function(benchmark::State& state) {
  boost::function<size_t(size_t)> f1 = [](size_t mod)->size_t{
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod == 1) {
        ret += i;
      }
    }
    return ret;
  };
  boost::function<size_t(size_t)> f2 = [](size_t mod)->size_t{
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod != 1) {
        ret += i;
      }
    }
    return ret;
  };
  static_assert(std::is_same_v<decltype(f1), decltype(f2)>);
  for (auto _ : state) {
    auto ret = f1(3) + f2(5);
    benchmark::DoNotOptimize(ret);
  }
}

static void BM_native_lambda_type(benchmark::State& state) {
  auto f1 = [](size_t mod)->size_t{
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod == 1) {
        ret += i;
      }
    }
    return ret;
  };
  auto f2 = [](size_t mod)->size_t{
    size_t ret = 0;
    for (size_t i = 0; i < max; ++i) {
      if (i % mod != 1) {
        ret += i;
      }
    }
    return ret;
  };
  static_assert(!std::is_same_v<decltype(f1), decltype(f2)>);
  for (auto _ : state) {
    auto ret = f1(3) + f2(5);
    benchmark::DoNotOptimize(ret);
  }
}


BENCHMARK(BM_virtual)->Unit(benchmark::kMillisecond);

BENCHMARK(BM_std_function)->Unit(benchmark::kMillisecond);

BENCHMARK(BM_boost_function)->Unit(benchmark::kMillisecond);

BENCHMARK(BM_native_lambda_type)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

