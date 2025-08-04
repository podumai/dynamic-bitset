#pragma once

#include <numeric>
#include <vector>
#include <benchmark/benchmark.h>
#include <cstdint>

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

}

template<long long Start, long long Limit, int Step>
auto DenseGenerator(::benchmark::internal::Benchmark* b) -> void
{
  b->DenseRange(Start, Limit, Step);
}

template<int Multiplier, long long Start, long long Limit>
auto MultiplicativeGenerator(::benchmark::internal::Benchmark* b) -> void
{
  b->RangeMultiplier(Multiplier)->Range(Start, Limit);
}

}

template<typename Container>
auto BM_DefaultConstructor(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    Container unit;
    ::benchmark::DoNotOptimize(unit);
  }
}

template<typename Container>
auto BM_CopyConstructor(::benchmark::State& state) -> void
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
auto BM_MoveConstructor(::benchmark::State& state) -> void
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
auto BM_CopyAssignment(::benchmark::State& state) -> void
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
auto BM_MoveAssignment(::benchmark::State& state) -> void
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
auto BM_PushBack(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit;
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      unit.push_back(!(i & 1));
    }
  }
}

template<typename Container>
auto BM_PopBack(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      unit.pop_back();
    }
  }
}

template<typename Container>
auto BM_Front(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit1(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit1.front());
  }
}

template<typename Container>
auto BM_Back(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    ::benchmark::DoNotOptimize(unit.back());
  }
}

template<typename Container>
auto BM_SubscriptTraverse(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      ::benchmark::DoNotOptimize(unit[i]);
    }
  }
}

template<typename Container>
auto BM_AtTraverse(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      ::benchmark::DoNotOptimize(unit.at(i));
    }
  }
}

template<typename Container>
auto BM_TestTraverse(::benchmark::State& state) -> void
{
  for (auto _ : state)
  {
    state.PauseTiming();
    Container unit(state.range(0));
    state.ResumeTiming();
    for (decltype(state.range(0)) i{}; i < state.range(0); ++i)
    {
      ::benchmark::DoNotOptimize(unit.test(i));
    }
  }
}

template<typename Container>
auto BM_BitwiseAND(::benchmark::State& state) -> void
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
auto BM_BitwiseOR(::benchmark::State& state) -> void
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
auto BM_BitwiseXOR(::benchmark::State& state) -> void
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
auto BM_BitwiseLeftShift(::benchmark::State& state) -> void
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
auto BM_BitwiseRightShift(::benchmark::State& state) -> void
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

}

#define BITS_DefaultDenseRangeGenerator bits::benchmark::generators::DenseGenerator<bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange, bits::benchmark::generators::DefaultDenseStep>
#define BITS_DefaultRangeGenerator bits::benchmark::generators::MultiplicativeGenerator<bits::benchmark::generators::DefaultMultiplierRange, bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange>

#define BITS_BenchmarkNameGenerator(container, func) "[" #container "::" #func "]"

#define BITS_DefaultConstructorBenchmark(container, func) BENCHMARK(bits::benchmark::BM_DefaultConstructor<container>)->Name(BITS_BenchmarkNameGenerator(container, func))
#define BITS_CopyConstructorBenchmark(container, func) BENCHMARK(bits::benchmark::BM_CopyConstructor<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Apply(BITS_DefaultRangeGenerator)
#define BITS_MoveConstructorBenchmark(container, func) BENCHMARK(bits::benchmark::BM_MoveConstructor<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Apply(BITS_DefaultRangeGenerator)
#define BITS_CopyAssignmentBenchmark(container, func) BENCHMARK(bits::benchmark::BM_CopyAssignment<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Apply(BITS_DefaultRangeGenerator)
#define BITS_MoveAssignmentBenchmark(container, func) BENCHMARK(bits::benchmark::BM_MoveAssignment<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Apply(BITS_DefaultRangeGenerator)
#define BITS_PushBackBenchmark(container, func, generator) BENCHMARK(bits::benchmark::BM_PushBack<container>)->Name(BITS_BenchmarkNameGenerator(container, func "@" #generator))->Apply(generator)
#define BITS_SubscriptLoopBenchmark(container, func) BENCHMARK(bits::benchmark::BM_SubscriptTraverse<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_AtLoopBenchmark(container, func) BENCHMARK(bits::benchmark::BM_AtTraverse<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_TestLoopBenchmark(container, func) BENCHMARK(bits::benchmark::BM_TestTraverse<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_AndOperatorBenchmark(container, func) BENCHMARK(bits::benchmark::BM_BitwiseAND<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_OrOperatorBenchmark(container, func) BENCHMARK(bits::benchmark::BM_BitwiseOR<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_XorOperatorBenchmark(container, func) BENCHMARK(bits::benchmark::BM_BitwiseXOR<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->Range(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange)
#define BITS_LeftShiftBenchmark(container, func) BENCHMARK(bits::benchmark::BM_BitwiseLeftShift<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->ArgsProduct({benchmark::CreateRange(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange), benchmark::CreateRange(1, bits::benchmark::generators::DefaultLimitRange, bits::benchmark::generators::DefaultMultiplierRange)})
#define BITS_RightShiftBenchmark(container, func) BENCHMARK(bits::benchmark::BM_BitwiseRightShift<container>)->Name(BITS_BenchmarkNameGenerator(container, func))->ArgsProduct({benchmark::CreateRange(bits::benchmark::generators::DefaultStartRange, bits::benchmark::generators::DefaultLimitRange), benchmark::CreateRange(1, bits::benchmark::generators::DefaultLimitRange, bits::benchmark::generators::DefaultMultiplierRange)})