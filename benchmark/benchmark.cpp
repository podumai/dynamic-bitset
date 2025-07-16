#include <benchmark/benchmark.h>
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <boost/dynamic_bitset.hpp>
#include <vector>

#if defined(BITS_DYNAMIC_BITSET_TEST)
  #define TEST_UNIT bits::DynamicBitset<size_t>
  #define PUSH_BACK(EXPR) PushBack(EXPR)
#elif defined(BOOST_DYNAMIC_BITSET_TEST)
  #define TEST_UNIT boost::dynamic_bitset<size_t>
#elif defined(STD_VECTOR_BOOL_TEST)
  #define TEST_UNIT std::vector<bool>
#elif defined(UNKNOWN_TEST_UNIT)
  #define TEST_UNIT void
#endif

#if defined(BOOST_DYNAMIC_BITSET_TEST) || defined(STD_VECTOR_BOOL_TEST)
  #define PUSH_BACK(EXPR) push_back(EXPR)
#endif

namespace
{

auto EmptyTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    TEST_UNIT unit;
  }
}

auto CopyTest(benchmark::State& state) -> void
{
  TEST_UNIT unit(1'000'000);
  for (auto _ : state)
  {
    TEST_UNIT copiedUnit(unit);
  }
}

auto EmptyPushBackTest(benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    TEST_UNIT unit;
    for (int i{}; i < 1'000'000; ++i)
    {
      unit.PUSH_BACK((i & 1) == 0);
    }
  }
}

auto SubscriptTraverseTest(benchmark::State& state) -> void
{
  TEST_UNIT unit(1'000'000);
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
  TEST_UNIT unit(1'000'000);
  TEST_UNIT anotherUnit(1'000'000);
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