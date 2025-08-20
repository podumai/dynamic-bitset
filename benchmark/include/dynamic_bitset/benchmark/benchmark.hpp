#pragma once

#include <benchmark/benchmark.h>

#include <cstdint>
#include <numeric>
#include <vector>

#if defined(BITS_DYNAMIC_BITSET_BENCHMARK)
  #define SIZE_METHOD() Size()
  #define CAPACITY_METHOD() Capacity()
  #define PUSH_BACK_METHOD(bit) PushBack(bit)
  #define POP_BACK_METHOD() PopBack()
  #define AT_METHOD(index) At(index)
  #define TEST_METHOD(index) Test(index)
  #define SET_METHOD() Set()
  #define RESET_METHOD() Reset()
  #define FLIP_METHOD() Flip()
  #define FRONT_METHOD() Front()
  #define BACK_METHOD() Back()
  #define COUNT_METHOD() Count()
  #define SWAP_METHOD(bitset) Swap(bitset)
  #define TO_STRING_METHOD() ToString()
  #define EMPTY_METHOD() Empty()
  #define ANY_METHOD() Any()
  #define ALL_METHOD() All()
  #define NONE_METHOD() None()
#else
  #define SIZE_METHOD() size()
  #define CAPACITY_METHOD() capacity()
  #define PUSH_BACK_METHOD(bit) push_back(bit)
  #define POP_BACK_METHOD() pop_back()
  #define AT_METHOD(index) at(index)
  #define TEST_METHOD(index) test(index)
  #define SET_METHOD() set()
  #define RESET_METHOD() reset()
  #define FLIP_METHOD() flip()
  #define FRONT_METHOD() front()
  #define BACK_METHOD() back()
  #define COUNT_METHOD() count()
  #define SWAP_METHOD(bitset) swap(bitset)
  #define TO_STRING_METHOD() to_string()
  #define EMPTY_METHOD() empty()
  #define ANY_METHOD() any()
  #define ALL_METHOD() all()
  #define NONE_METHOD() none()
#endif

namespace bits::benchmark
{

namespace generators
{

inline namespace __details
{

constexpr long long DefaultStartRange{2};
constexpr long long DefaultLimitRange{INT_MAX};
constexpr int DefaultMultiplierRange{2};
constexpr int DefaultDenseStep{200'000'039};

}  // namespace __details

template<long long Start, long long Limit, int Step>
auto DenseGenerator(
  ::benchmark::internal::Benchmark* b
) -> void
{
  b->DenseRange(Start, Limit, Step);
}

template<int Multiplier, long long Start, long long Limit>
auto MultiplicativeGenerator(
  ::benchmark::internal::Benchmark* b
) -> void
{
  b->RangeMultiplier(Multiplier)->Range(Start, Limit);
}

}  // namespace generators

template<typename Container>
auto BM_DefaultConstructor(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    Container unit;
    ::benchmark::DoNotOptimize(unit);
  }
}

template<typename Container>
auto BM_CopyConstructor(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    Container copiedUnit(unit);
    ::benchmark::DoNotOptimize(copiedUnit);
  }
}

template<typename Container>
auto BM_MoveConstructor(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    Container movedUnit(std::move(unit));
    ::benchmark::DoNotOptimize(movedUnit);
  }
}

template<typename Container>
auto BM_CopyAssignment(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit1(state.range(0));
    Container unit2;
    state.ResumeTiming();
    unit2 = unit1;
    ::benchmark::DoNotOptimize(unit2);
  }
}

template<typename Container>
auto BM_MoveAssignment(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit1(state.range(0));
    Container unit2;
    state.ResumeTiming();
    unit2 = std::move(unit1);
    ::benchmark::DoNotOptimize(unit2);
  }
}

template<typename Container>
auto BM_PushBack(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit;
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      unit.PUSH_BACK_METHOD(!(i & 1));
    }
  }
}

template<typename Container>
auto BM_PopBack(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      unit.POP_BACK_METHOD();
    }
  }
}

template<typename Container>
auto BM_Front(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit1(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit1.FRONT_METHOD());
  }
}

template<typename Container>
auto BM_Back(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit.BACK_METHOD());
  }
}

template<typename Container>
auto BM_SubscriptTraverse(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      bool bit_value{unit[i]};
      ::benchmark::DoNotOptimize(bit_value);
    }
  }
}

template<typename Container>
auto BM_AtTraverse(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      bool bit_value{unit.AT_METHOD(i)};
      ::benchmark::DoNotOptimize(bit_value);
    }
  }
}

template<typename Container>
auto BM_TestTraverse(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      bool bit_value{unit.TEST_METHOD(i)};
      ::benchmark::DoNotOptimize(bit_value);
    }
  }
}

template<typename Container>
auto BM_Count(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    decltype(unit.COUNT_METHOD()) set_bits{unit.COUNT_METHOD()};
    ::benchmark::DoNotOptimize(set_bits);
  }
}

template<typename Container>
auto BM_Empty(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    bool is_empty{unit.EMPTY_METHOD()};
    ::benchmark::DoNotOptimize(is_empty);
  }
}

template<typename Container>
auto BM_All(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit.ALL_METHOD());
  }
}

template<typename Container>
auto BM_Any(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit.ANY_METHOD());
  }
}

template<typename Container>
auto BM_None(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit.NONE_METHOD());
  }
}

template<typename Container>
auto BM_Set(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    unit.SET_METHOD();
    ::benchmark::DoNotOptimize(unit);
  }
}

template<typename Container>
auto BM_Reset(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    unit.RESET_METHOD();
    ::benchmark::DoNotOptimize(unit);
  }
}

template<typename Container>
auto BM_Flip(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    unit.FLIP_METHOD();
    ::benchmark::DoNotOptimize(unit);
  }
}

template<typename Container>
auto BM_ToString(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit.TO_STRING_METHOD());
  }
}

template<typename Container>
auto BM_Inverse(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(~unit);
  }
}

template<typename Container>
auto BM_Swap(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container a(state.range(0));
    Container b;
    state.ResumeTiming();
    b.SWAP_METHOD(a);
    ::benchmark::DoNotOptimize(b);
  }
}

template<typename Container>
auto BM_Size(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    decltype(unit.SIZE_METHOD()) unit_size(unit.SIZE_METHOD());
    ::benchmark::DoNotOptimize(unit_size);
  }
}

template<typename Container>
auto BM_Capacity(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    decltype(unit.CAPACITY_METHOD()) unit_capacity(unit.CAPACITY_METHOD());
    ::benchmark::DoNotOptimize(unit_capacity);
  }
}

template<typename Container>
auto BM_BitwiseAND(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit1(state.range(0));
    Container unit2(state.range(0));
    state.ResumeTiming();
    unit1 &= unit2;
  }
}

template<typename Container>
auto BM_BitwiseOR(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit1(state.range(0));
    Container unit2(state.range(0));
    state.ResumeTiming();
    unit1 |= unit2;
  }
}

template<typename Container>
auto BM_BitwiseXOR(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit1(state.range(0));
    Container unit2(state.range(0));
    state.PauseTiming();
    unit1 ^= unit2;
  }
}

template<typename Container>
auto BM_BitwiseLeftShift(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    unit <<= state.range(1);
    ::benchmark::DoNotOptimize(unit);
  }
}

template<typename Container>
auto BM_BitwiseRightShift(
  ::benchmark::State& state
) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    unit >>= state.range(1);
    ::benchmark::DoNotOptimize(unit);
  }
}

}  // namespace bits::benchmark

#define BITS_DefaultDenseRangeGenerator             \
  bits::benchmark::generators::DenseGenerator<      \
    bits::benchmark::generators::DefaultStartRange, \
    bits::benchmark::generators::DefaultLimitRange, \
    bits::benchmark::generators::DefaultDenseStep>
#define BITS_DefaultRangeGenerator                       \
  bits::benchmark::generators::MultiplicativeGenerator<  \
    bits::benchmark::generators::DefaultMultiplierRange, \
    bits::benchmark::generators::DefaultStartRange,      \
    bits::benchmark::generators::DefaultLimitRange>

#define BITS_BenchmarkNameGenerator(container, func) "[" #container "::" #func "]"

#define BITS_DefaultConstructorBenchmark(container, func) \
  BENCHMARK(bits::benchmark::BM_DefaultConstructor<container>)->Name(BITS_BenchmarkNameGenerator(container, func))
#define BITS_CopyConstructorBenchmark(container, func)      \
  BENCHMARK(bits::benchmark::BM_CopyConstructor<container>) \
    ->Name(BITS_BenchmarkNameGenerator(container, func))    \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_MoveConstructorBenchmark(container, func)      \
  BENCHMARK(bits::benchmark::BM_MoveConstructor<container>) \
    ->Name(BITS_BenchmarkNameGenerator(container, func))    \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_CopyAssignmentBenchmark(container, func)      \
  BENCHMARK(bits::benchmark::BM_CopyAssignment<container>) \
    ->Name(BITS_BenchmarkNameGenerator(container, func))   \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_MoveAssignmentBenchmark(container, func)      \
  BENCHMARK(bits::benchmark::BM_MoveAssignment<container>) \
    ->Name(BITS_BenchmarkNameGenerator(container, func))   \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_PushBackBenchmark(container, func, generator)              \
  BENCHMARK(bits::benchmark::BM_PushBack<container>)                    \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(generator)
#define BITS_PopBackBenchmark(container, func)           \
  BENCHMARK(bits::benchmark::BM_PopBack<container>)      \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_SubscriptLoopBenchmark(container, func)          \
  BENCHMARK(bits::benchmark::BM_SubscriptTraverse<container>) \
    ->Name(BITS_BenchmarkNameGenerator(container, func))      \
    ->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_AtLoopBenchmark(container, func)            \
  BENCHMARK(bits::benchmark::BM_AtTraverse<container>)   \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_TestLoopBenchmark(container, func)          \
  BENCHMARK(bits::benchmark::BM_TestTraverse<container>) \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_FrontBenchmark(container, func)             \
  BENCHMARK(bits::benchmark::BM_Front<container>)        \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_BackBenchmark(container, func)              \
  BENCHMARK(bits::benchmark::BM_Back<container>)         \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_CountBenchmark(container, func)             \
  BENCHMARK(bits::benchmark::BM_Count<container>)        \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_EmptyBenchmark(container, func)             \
  BENCHMARK(bits::benchmark::BM_Empty<container>)        \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_ToStringBenchmark(container, func)          \
  BENCHMARK(bits::benchmark::BM_ToString<container>)     \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_AllBenchmark(container, func)               \
  BENCHMARK(bits::benchmark::BM_All<container>)          \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_AnyBenchmark(container, func)               \
  BENCHMARK(bits::benchmark::BM_Any<container>)          \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_NoneBenchmark(container, func)              \
  BENCHMARK(bits::benchmark::BM_None<container>)         \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_InverseBenchmark(container, func)           \
  BENCHMARK(bits::benchmark::BM_Inverse<container>)      \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_SetBenchmark(container, func)               \
  BENCHMARK(bits::benchmark::BM_Set<container>)          \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_ResetBenchmark(container, func)             \
  BENCHMARK(bits::benchmark::BM_Reset<container>)        \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_FlipBenchmark(container, func)              \
  BENCHMARK(bits::benchmark::BM_Flip<container>)         \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_SwapBenchmark(container, func)              \
  BENCHMARK(bits::benchmark::BM_Swap<container>)         \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_SizeBenchmark(container, func)              \
  BENCHMARK(bits::benchmark::BM_Size<container>)         \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_CapacityBenchmark(container, func)          \
  BENCHMARK(bits::benchmark::BM_Capacity<container>)     \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Apply(BITS_DefaultRangeGenerator)
#define BITS_AndOperatorBenchmark(container, func)       \
  BENCHMARK(bits::benchmark::BM_BitwiseAND<container>)   \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_OrOperatorBenchmark(container, func)        \
  BENCHMARK(bits::benchmark::BM_BitwiseOR<container>)    \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_XorOperatorBenchmark(container, func)       \
  BENCHMARK(bits::benchmark::BM_BitwiseXOR<container>)   \
    ->Name(BITS_BenchmarkNameGenerator(container, func)) \
    ->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_LeftShiftBenchmark(container, func)                                                                \
  BENCHMARK(bits::benchmark::BM_BitwiseLeftShift<container>)                                                    \
    ->Name(BITS_BenchmarkNameGenerator(container, func))                                                        \
    ->ArgsProduct(                                                                                              \
      {benchmark::CreateRange(                                                                                  \
         bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange         \
       ),                                                                                                       \
       benchmark::CreateRange(                                                                                  \
         1, bits::benchmark::generators::DefaultLimitRange, bits::benchmark::generators::DefaultMultiplierRange \
       )}                                                                                                       \
    )
#define BITS_RightShiftBenchmark(container, func)                                                               \
  BENCHMARK(bits::benchmark::BM_BitwiseRightShift<container>)                                                   \
    ->Name(BITS_BenchmarkNameGenerator(container, func))                                                        \
    ->ArgsProduct(                                                                                              \
      {benchmark::CreateRange(                                                                                  \
         bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange         \
       ),                                                                                                       \
       benchmark::CreateRange(                                                                                  \
         1, bits::benchmark::generators::DefaultLimitRange, bits::benchmark::generators::DefaultMultiplierRange \
       )}                                                                                                       \
    )