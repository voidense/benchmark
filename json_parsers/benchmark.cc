#include <benchmark/benchmark.h>

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
#include "rapidjson/document.h"
#include "simdjson.h"


std::string loadJsonFile(const std::string_view filepath) {
  std::ifstream filestream(filepath);
  std::string res({std::istreambuf_iterator<char>(filestream)},
                  std::istreambuf_iterator<char>());
  return res;
}

static void BM_nlohmann_json(benchmark::State& state) {
  const std::string jsonStr = loadJsonFile("./test.json");
  for (auto _ : state) {
    auto j = nlohmann::json::parse(jsonStr);
  }
}

static void BM_rapidjson(benchmark::State& state) {
  const std::string jsonStr = loadJsonFile("./test.json");
  for (auto _ : state) {
    rapidjson::Document doc;
    rapidjson::ParseResult res = doc.Parse(jsonStr.c_str());
    (void)res;
  }
}

static void BM_simdjson(benchmark::State& state) {
  const std::string jsonStr = loadJsonFile("./test.json");
  for (auto _ : state) {
    simdjson::dom::parser parser;
    simdjson::dom::element res = parser.parse(jsonStr);
    (void)res;
  }
}


BENCHMARK(BM_nlohmann_json);

BENCHMARK(BM_rapidjson);

BENCHMARK(BM_simdjson);

BENCHMARK_MAIN();

/* sample numbers

2020-06-27 18:49:11
Running ./bm_json_parsers
Run on (8 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 6.54, 4.01, 3.39
-----------------------------------------------------------
Benchmark                 Time             CPU   Iterations
-----------------------------------------------------------
BM_nlohmann_json     309236 ns       308551 ns         2274
BM_rapidjson          48324 ns        48250 ns        14526
BM_simdjson           10593 ns        10552 ns        65662

*/
