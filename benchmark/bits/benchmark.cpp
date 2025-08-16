#include <benchmark/benchmark.h>

#include <dynamic_bitset/benchmark/benchmark.hpp>
#include <dynamic_bitset/dynamic_bitset.hpp>

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

BITS_PushBackBenchmark(BITS_DB(unsigned char), PushBack, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned char), PushBack, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned short), PushBack, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned short), PushBack, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned), PushBack, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned), PushBack, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long), PushBack, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long), PushBack, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long long), PushBack, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BITS_DB(unsigned long long), PushBack, BITS_DefaultDenseRangeGenerator);

BITS_PopBackBenchmark(BITS_DB(unsigned char), PopBack);
BITS_PopBackBenchmark(BITS_DB(unsigned short), PopBack);
BITS_PopBackBenchmark(BITS_DB(unsigned), PopBack);
BITS_PopBackBenchmark(BITS_DB(unsigned long), PopBack);
BITS_PopBackBenchmark(BITS_DB(unsigned long long), PopBack);

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

BITS_AtLoopBenchmark(BITS_DB(unsigned char), At);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned char), At);
BITS_AtLoopBenchmark(BITS_DB(unsigned short), At);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned short), At);
BITS_AtLoopBenchmark(BITS_DB(unsigned), At);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned), At);
BITS_AtLoopBenchmark(BITS_DB(unsigned long), At);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned long), At);
BITS_AtLoopBenchmark(BITS_DB(unsigned long long), At);
BITS_AtLoopBenchmark(BITS_CONST_DB(unsigned long long), At);

BITS_TestLoopBenchmark(BITS_DB(unsigned char), Test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned char), Test);
BITS_TestLoopBenchmark(BITS_DB(unsigned short), Test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned short), Test);
BITS_TestLoopBenchmark(BITS_DB(unsigned), Test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned), Test);
BITS_TestLoopBenchmark(BITS_DB(unsigned long), Test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned long), Test);
BITS_TestLoopBenchmark(BITS_DB(unsigned long long), Test);
BITS_TestLoopBenchmark(BITS_CONST_DB(unsigned long long), Test);

BITS_FrontBenchmark(BITS_DB(unsigned char), Front);
BITS_FrontBenchmark(BITS_CONST_DB(unsigned char), Front);
BITS_FrontBenchmark(BITS_DB(unsigned short), Front);
BITS_FrontBenchmark(BITS_CONST_DB(unsigned short), Front);
BITS_FrontBenchmark(BITS_DB(unsigned), Front);
BITS_FrontBenchmark(BITS_CONST_DB(unsigned long), Front);
BITS_FrontBenchmark(BITS_DB(unsigned long), Front);
BITS_FrontBenchmark(BITS_CONST_DB(unsigned long long), Front);
BITS_FrontBenchmark(BITS_DB(unsigned long long), Front);

BITS_BackBenchmark(BITS_DB(unsigned char), Back);
BITS_BackBenchmark(BITS_CONST_DB(unsigned char), Back);
BITS_BackBenchmark(BITS_DB(unsigned short), Back);
BITS_BackBenchmark(BITS_CONST_DB(unsigned short), Back);
BITS_BackBenchmark(BITS_DB(unsigned), Back);
BITS_BackBenchmark(BITS_CONST_DB(unsigned), Back);
BITS_BackBenchmark(BITS_DB(unsigned long), Back);
BITS_BackBenchmark(BITS_CONST_DB(unsigned long), Back);
BITS_BackBenchmark(BITS_DB(unsigned long long), Back);
BITS_BackBenchmark(BITS_CONST_DB(unsigned long long), Back);

BITS_CountBenchmark(BITS_DB(unsigned char), Count);
BITS_CountBenchmark(BITS_DB(unsigned short), Count);
BITS_CountBenchmark(BITS_DB(unsigned), Count);
BITS_CountBenchmark(BITS_DB(unsigned long), Count);
BITS_CountBenchmark(BITS_DB(unsigned long long), Count);

BITS_EmptyBenchmark(BITS_DB(unsigned char), Empty);
BITS_EmptyBenchmark(BITS_DB(unsigned short), Empty);
BITS_EmptyBenchmark(BITS_DB(unsigned), Empty);
BITS_EmptyBenchmark(BITS_DB(unsigned long), Empty);
BITS_EmptyBenchmark(BITS_DB(unsigned long long), Empty);

BITS_ToStringBenchmark(BITS_DB(unsigned char), ToString);
BITS_ToStringBenchmark(BITS_DB(unsigned short), ToString);
BITS_ToStringBenchmark(BITS_DB(unsigned), ToString);
BITS_ToStringBenchmark(BITS_DB(unsigned long), ToString);
BITS_ToStringBenchmark(BITS_DB(unsigned long long), ToString);

BITS_SetBenchmark(BITS_DB(unsigned char), Set);
BITS_SetBenchmark(BITS_DB(unsigned short), Set);
BITS_SetBenchmark(BITS_DB(unsigned), Set);
BITS_SetBenchmark(BITS_DB(unsigned long), Set);
BITS_SetBenchmark(BITS_DB(unsigned long long), Set);

BITS_ResetBenchmark(BITS_DB(unsigned char), Reset);
BITS_ResetBenchmark(BITS_DB(unsigned short), Reset);
BITS_ResetBenchmark(BITS_DB(unsigned), Reset);
BITS_ResetBenchmark(BITS_DB(unsigned long), Reset);
BITS_ResetBenchmark(BITS_DB(unsigned long long), Reset);

BITS_FlipBenchmark(BITS_DB(unsigned char), Flip);
BITS_FlipBenchmark(BITS_DB(unsigned short), Flip);
BITS_FlipBenchmark(BITS_DB(unsigned), Flip);
BITS_FlipBenchmark(BITS_DB(unsigned long), Flip);
BITS_FlipBenchmark(BITS_DB(unsigned long long), Flip);

BITS_SwapBenchmark(BITS_DB(unsigned char), Swap);
BITS_SwapBenchmark(BITS_DB(unsigned short), Swap);
BITS_SwapBenchmark(BITS_DB(unsigned), Swap);
BITS_SwapBenchmark(BITS_DB(unsigned long), Swap);
BITS_SwapBenchmark(BITS_DB(unsigned long long), Swap);

BITS_AllBenchmark(BITS_DB(unsigned char), All);
BITS_AllBenchmark(BITS_DB(unsigned short), All);
BITS_AllBenchmark(BITS_DB(unsigned), All);
BITS_AllBenchmark(BITS_DB(unsigned long), All);
BITS_AllBenchmark(BITS_DB(unsigned long long), All);

BITS_AnyBenchmark(BITS_DB(unsigned char), Any);
BITS_AnyBenchmark(BITS_DB(unsigned short), Any);
BITS_AnyBenchmark(BITS_DB(unsigned), Any);
BITS_AnyBenchmark(BITS_DB(unsigned long), Any);
BITS_AnyBenchmark(BITS_DB(unsigned long long), Any);

BITS_NoneBenchmark(BITS_DB(unsigned char), None);
BITS_NoneBenchmark(BITS_DB(unsigned short), None);
BITS_NoneBenchmark(BITS_DB(unsigned), None);
BITS_NoneBenchmark(BITS_DB(unsigned long), None);
BITS_NoneBenchmark(BITS_DB(unsigned long long), None);

BITS_SizeBenchmark(BITS_DB(unsigned char), Size);
BITS_SizeBenchmark(BITS_DB(unsigned short), Size);
BITS_SizeBenchmark(BITS_DB(unsigned), Size);
BITS_SizeBenchmark(BITS_DB(unsigned long), Size);
BITS_SizeBenchmark(BITS_DB(unsigned long long), Size);

BITS_CapacityBenchmark(BITS_DB(unsigned char), Capacity);
BITS_CapacityBenchmark(BITS_DB(unsigned short), Capacity);
BITS_CapacityBenchmark(BITS_DB(unsigned), Capacity);
BITS_CapacityBenchmark(BITS_DB(unsigned long), Capacity);
BITS_CapacityBenchmark(BITS_DB(unsigned long long), Capacity);

BITS_InverseBenchmark(BITS_DB(unsigned char), operator~);
BITS_InverseBenchmark(BITS_DB(unsigned short), operator~);
BITS_InverseBenchmark(BITS_DB(unsigned), operator~);
BITS_InverseBenchmark(BITS_DB(unsigned long), operator~);
BITS_InverseBenchmark(BITS_DB(unsigned long long), operator~);

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