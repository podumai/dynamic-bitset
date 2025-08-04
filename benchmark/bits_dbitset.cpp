#include <benchmark/benchmark.h>
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <dynamic_bitset/benchmark/benchmark.hpp>

#define BITS_DB(type) bits::DynamicBitset<type>
#define BITS_CONST_DB(type) const bits::DynamicBitset<type>

BITS_DefaultConstructorBenchmark(BITS_DB(unsigned char), DynamicBitset());
BITS_DefaultConstructorBenchmark(BITS_DB(unsigned short), DynamicBitset());
BITS_DefaultConstructorBenchmark(BITS_DB(unsigned), DynamicBitset());
BITS_DefaultConstructorBenchmark(BITS_DB(unsigned long), DynamicBitset());
BITS_DefaultConstructorBenchmark(BITS_DB(unsigned long long), DynamicBitset());

BITS_CopyConstructorBenchmark(BITS_DB(unsigned char), DynamicBitset(DynamicBitset&));
BITS_CopyConstructorBenchmark(BITS_DB(unsigned short), DynamicBitset(DynamicBitset&));
BITS_CopyConstructorBenchmark(BITS_DB(unsigned), DynamicBitset(DynamicBitset&));
BITS_CopyConstructorBenchmark(BITS_DB(unsigned long), DynamicBitset(DynamicBitset&));
BITS_CopyConstructorBenchmark(BITS_DB(unsigned long long), DynamicBitset(DynamicBitset&));

BITS_MoveConstructorBenchmark(BITS_DB(unsigned char), DynamicBitset(DynamicBitset&&));
BITS_MoveConstructorBenchmark(BITS_DB(unsigned short), DynamicBitset(DynamicBitset&&));
BITS_MoveConstructorBenchmark(BITS_DB(unsigned), DynamicBitset(DynamicBitset&&));
BITS_MoveConstructorBenchmark(BITS_DB(unsigned long), DynamicBitset(DynamicBitset&&));
BITS_MoveConstructorBenchmark(BITS_DB(unsigned long long), DynamicBitset(DynamicBitset&&));

BITS_CopyAssignmentBenchmark(BITS_DB(unsigned char), operator=);
BITS_CopyAssignmentBenchmark(BITS_DB(unsigned short), operator=);
BITS_CopyAssignmentBenchmark(BITS_DB(unsigned), operator=);
BITS_CopyAssignmentBenchmark(BITS_DB(unsigned long), operator=);
BITS_CopyAssignmentBenchmark(BITS_DB(unsigned long long), operator=);

BITS_MoveAssignmentBenchmark(BITS_DB(unsigned char), operator=);
BITS_MoveAssignmentBenchmark(BITS_DB(unsigned short), operator=);
BITS_MoveAssignmentBenchmark(BITS_DB(unsigned), operator=);
BITS_MoveAssignmentBenchmark(BITS_DB(unsigned long), operator=);
BITS_MoveAssignmentBenchmark(BITS_DB(unsigned long long), operator=);

BITS_PushBackBenchmark(BITS_DB(unsigned char), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned char), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned short), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned short), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long long), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long long), push_back, BITS_DefaultDenseRangeGenerator);

BITS_SubscriptLoopBenchmark(BITS_DB(unsigned char), operator[]);
BITS_SubscriptLoopBenchmark(BITS_CONST_DB(unsigned char), operator[]);
BITS_SubscriptLoopBenchmark(BITS_DB(unsigned short), operator[]);
BITS_SubscriptLoopBenchmark(BITS_CONST_DB(unsigned short), operator[]);
BITS_SubscriptLoopBenchmark(BITS_DB(unsigned), operator[]);
BITS_SubscriptLoopBenchmark(BITS_CONST_DB(unsigned), operator[]);
BITS_SubscriptLoopBenchmark(BITS_DB(unsigned long), operator[]);
BITS_SubscriptLoopBenchmark(BITS_CONST_DB(unsigned long), operator[]);
BITS_SubscriptLoopBenchmark(BITS_DB(unsigned long long), operator[]);
BITS_SubscriptLoopBenchmark(BITS_CONST_DB(unsigned long long), operator[]);

BITS_AtLoopBenchmark(BITS_DB(unsigned char), at);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned char), at);
BITS_AtLoopBenchmark(BITS_DB(unsigned short), at);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned short), at);
BITS_AtLoopBenchmark(BITS_DB(unsigned), at);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned), at);
BITS_AtLoopBenchmark(BITS_DB(unsigned long), at);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned long), at);
BITS_AtLoopBenchmark(BITS_DB(unsigned long long), at);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned long long), at);

BITS_TestLoopBenchmark(BITS_DB(unsigned char), test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned char), test);
BITS_TestLoopBenchmark(BITS_DB(unsigned short), test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned short), test);
BITS_TestLoopBenchmark(BITS_DB(unsigned), test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned), test);
BITS_TestLoopBenchmark(BITS_DB(unsigned long), test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned long), test);
BITS_TestLoopBenchmark(BITS_DB(unsigned long long), test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned long long), test);

BITS_AndOperatorBenchmark(BITS_DB(unsigned char), operator&=);
BITS_AndOperatorBenchmark(BITS_DB(unsigned short), operator&=);
BITS_AndOperatorBenchmark(BITS_DB(unsigned), operator&=);
BITS_AndOperatorBenchmark(BITS_DB(unsigned long), operator&=);
BITS_AndOperatorBenchmark(BITS_DB(unsigned long long), operator&=);

BITS_OrOperatorBenchmark(BITS_DB(unsigned char), operator|=);
BITS_OrOperatorBenchmark(BITS_DB(unsigned short), operator|=);
BITS_OrOperatorBenchmark(BITS_DB(unsigned), operator|=);
BITS_OrOperatorBenchmark(BITS_DB(unsigned long), operator|=);
BITS_OrOperatorBenchmark(BITS_DB(unsigned long long), operator|=);

BITS_XorOperatorBenchmark(BITS_DB(unsigned char), operator^=);
BITS_XorOperatorBenchmark(BITS_DB(unsigned short), operator^=);
BITS_XorOperatorBenchmark(BITS_DB(unsigned), operator^=);
BITS_XorOperatorBenchmark(BITS_DB(unsigned long), operator^=);
BITS_XorOperatorBenchmark(BITS_DB(unsigned long long), operator^=);

BENCHMARK_MAIN();