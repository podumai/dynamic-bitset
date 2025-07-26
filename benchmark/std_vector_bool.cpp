#include <benchmark/benchmark.h>
#include <vector>

namespace
{

auto EmptyTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    std::vector<bool> unit;
  }
}

auto CopyTest(benchmark::State& state) -> void
{
  std::vector<bool> unit(1'000'000);
  for (auto _ : state)
  {
    std::vector<bool> copiedUnit{unit};
  }
}

auto EmptyPushBackTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    std::vector<bool> unit;
    for (int i{}; i < 1'000'000; ++i)
    {
      unit.push_back(!(i & 1));
    }
  }
}

auto SubscriptTraverseTest(benchmark::State& state) -> void
{
  std::vector<bool> unit(1'000'000);
  for (auto _ : state)
  {
    for (int i{}; i < 1'000'000; ++i)
    {
      unit[i];
    }
  }
}

}

BENCHMARK(EmptyTest);
BENCHMARK(CopyTest);
BENCHMARK(EmptyPushBackTest);
BENCHMARK(SubscriptTraverseTest);
BENCHMARK_MAIN();