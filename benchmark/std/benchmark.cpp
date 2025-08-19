#include <benchmark/benchmark.h>
#include <dynamic_bitset/benchmark/benchmark.hpp>
#include <vector>

#define STD_DB typename std::vector<bool>
#define STD_CONST_DB const typename std::vector<bool>

BITS_DefaultConstructorBenchmark(STD_DB, std::vector());

BITS_CopyConstructorBenchmark(STD_DB, std::vector(const std::vector<bool>&));

BITS_MoveConstructorBenchmark(STD_DB, std::vector(std::vector<bool>&&));

BITS_CopyAssignmentBenchmark(STD_DB, operator=);

BITS_MoveAssignmentBenchmark(STD_DB, operator=);

BITS_PushBackBenchmark(STD_DB, push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(STD_DB, push_back, BITS_DefaultDenseRangeGenerator);

BITS_PopBackBenchmark(STD_DB, pop_back);

BITS_SubscriptLoopBenchmark(STD_DB, operator[]);
BITS_SubscriptLoopBenchmark(STD_CONST_DB, operator[]);

BITS_AtLoopBenchmark(STD_DB, at);
BITS_AtLoopBenchmark(STD_CONST_DB, at);

BITS_FrontBenchmark(STD_DB, front);
BITS_FrontBenchmark(STD_CONST_DB, front);

BITS_BackBenchmark(STD_DB, back);
BITS_BackBenchmark(STD_CONST_DB, back);

BITS_EmptyBenchmark(STD_DB, empty);

BITS_FlipBenchmark(STD_DB, flip);

BITS_SwapBenchmark(STD_DB, swap);

BITS_SizeBenchmark(STD_DB, size);

BITS_CapacityBenchmark(STD_DB, capacity);

BENCHMARK_MAIN();