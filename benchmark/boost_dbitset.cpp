#include <benchmark/benchmark.h>
#include <boost/dynamic_bitset.hpp>

namespace
{

auto EmptyTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    boost::dynamic_bitset unit;
  }
}

auto CopyTest(benchmark::State& state) -> void
{
  boost::dynamic_bitset<size_t> unit(1'000'000);
  for (auto _ : state)
  {
    boost::dynamic_bitset copiedUnit{unit};
  }
}

auto EmptyPushBackTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    boost::dynamic_bitset unit;
    for (int i{}; i < 1'000'000; ++i)
    {
      unit.push_back(!(i & 1));
    }
  }
}

auto SubscriptTraverseTest(benchmark::State& state) -> void
{
  boost::dynamic_bitset<size_t> unit(1'000'000);
  for (auto _ : state)
  {
    for (int i{}; i < 1'000'000; ++i)
    {
      unit[i];
    }
  }
}

auto BitwiseANDoperatorTest(benchmark::State& state) -> void
{
  boost::dynamic_bitset<size_t> unit(1'000'000);
  boost::dynamic_bitset<size_t> anotherUnit(1'000'000);
  for (auto _ : state)
  {
    unit &= anotherUnit;
  }
}

}

BENCHMARK(EmptyTest);
BENCHMARK(CopyTest);
BENCHMARK(EmptyPushBackTest);
BENCHMARK(SubscriptTraverseTest);
BENCHMARK(BitwiseANDoperatorTest);
BENCHMARK_MAIN();