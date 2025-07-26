#include <benchmark/benchmark.h>
#include <dynamic_bitset/dynamic_bitset.hpp>

namespace
{

auto EmptyTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    bits::DynamicBitset unit;
  }
}

auto CopyTest(benchmark::State& state) -> void
{
  bits::DynamicBitset unit{1'000'000};
  for (auto _ : state)
  {
    bits::DynamicBitset copiedUnit{unit};
  }
}

auto EmptyPushBackTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    bits::DynamicBitset unit;
    for (int i{}; i < 1'000'000; ++i)
    {
      unit.push_back(!(i & 1));
    }
  }
}

auto SubscriptTraverseTest(benchmark::State& state) -> void
{
  bits::DynamicBitset unit(1'000'000);
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
  bits::DynamicBitset unit(1'000'000);
  bits::DynamicBitset anotherUnit(1'000'000);
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