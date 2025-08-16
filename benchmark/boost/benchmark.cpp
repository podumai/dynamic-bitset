#include <benchmark/benchmark.h>
#include <dynamic_bitset/benchmark/benchmark.hpp>
#include <boost/dynamic_bitset.hpp>


#define BOOST_DB(type) boost::dynamic_bitset<type>
#define BOOST_CONST_DB(type) const boost::dynamic_bitset<type>

BITS_DefaultConstructorBenchmark(BOOST_DB(unsigned char), dynamic_bitset());
BITS_DefaultConstructorBenchmark(BOOST_DB(unsigned short), dynamic_bitset());
BITS_DefaultConstructorBenchmark(BOOST_DB(unsigned), dynamic_bitset());
BITS_DefaultConstructorBenchmark(BOOST_DB(unsigned long), dynamic_bitset());
BITS_DefaultConstructorBenchmark(BOOST_DB(unsigned long long), dynamic_bitset());

BITS_CopyConstructorBenchmark(BOOST_DB(unsigned char), dynamic_bitset(dynamic_bitset&));
BITS_CopyConstructorBenchmark(BOOST_DB(unsigned short), dynamic_bitset(dynamic_bitset&));
BITS_CopyConstructorBenchmark(BOOST_DB(unsigned), dynamic_bitset(dynamic_bitset&));
BITS_CopyConstructorBenchmark(BOOST_DB(unsigned long), dynamic_bitset(dynamic_bitset&));
BITS_CopyConstructorBenchmark(BOOST_DB(unsigned long long), dynamic_bitset(dynamic_bitset&));

BITS_MoveConstructorBenchmark(BOOST_DB(unsigned char), dynamic_bitset(dynamic_bitset&&));
BITS_MoveConstructorBenchmark(BOOST_DB(unsigned short), dynamic_bitset(dynamic_bitset&&));
BITS_MoveConstructorBenchmark(BOOST_DB(unsigned), dynamic_bitset(dynamic_bitset&&));
BITS_MoveConstructorBenchmark(BOOST_DB(unsigned long), dynamic_bitset(dynamic_bitset&&));
BITS_MoveConstructorBenchmark(BOOST_DB(unsigned long long), dynamic_bitset(dynamic_bitset&&));

BITS_CopyAssignmentBenchmark(BOOST_DB(unsigned char), operator=);
BITS_CopyAssignmentBenchmark(BOOST_DB(unsigned short), operator=);
BITS_CopyAssignmentBenchmark(BOOST_DB(unsigned), operator=);
BITS_CopyAssignmentBenchmark(BOOST_DB(unsigned long), operator=);
BITS_CopyAssignmentBenchmark(BOOST_DB(unsigned long long), operator=);

BITS_MoveAssignmentBenchmark(BOOST_DB(unsigned char), operator=);
BITS_MoveAssignmentBenchmark(BOOST_DB(unsigned short), operator=);
BITS_MoveAssignmentBenchmark(BOOST_DB(unsigned), operator=);
BITS_MoveAssignmentBenchmark(BOOST_DB(unsigned long), operator=);
BITS_MoveAssignmentBenchmark(BOOST_DB(unsigned long long), operator=);

BITS_PushBackBenchmark(BOOST_DB(unsigned char), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned char), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned short), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned short), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned long), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned long), push_back, BITS_DefaultDenseRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned long long), push_back, BITS_DefaultRangeGenerator);
BITS_PushBackBenchmark(BOOST_DB(unsigned long long), push_back, BITS_DefaultDenseRangeGenerator);

BITS_PopBackBenchmark(BOOST_DB(unsigned char), pop_back);
BITS_PopBackBenchmark(BOOST_DB(unsigned short), pop_back);
BITS_PopBackBenchmark(BOOST_DB(unsigned), pop_back);
BITS_PopBackBenchmark(BOOST_DB(unsigned long), pop_back);
BITS_PopBackBenchmark(BOOST_DB(unsigned long long), pop_back);

BITS_SubscriptLoopBenchmark(BOOST_DB(unsigned char), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_CONST_DB(unsigned char), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_DB(unsigned short), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_CONST_DB(unsigned short), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_DB(unsigned), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_CONST_DB(unsigned), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_DB(unsigned long), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_CONST_DB(unsigned long), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_DB(unsigned long long), operator[]);
BITS_SubscriptLoopBenchmark(BOOST_CONST_DB(unsigned long long), operator[]);

BITS_AtLoopBenchmark(BOOST_DB(unsigned char), at);
BITS_AtLoopBenchmark(BOOST_CONST_DB(unsigned char), at);
BITS_AtLoopBenchmark(BOOST_DB(unsigned short), at);
BITS_AtLoopBenchmark(BOOST_CONST_DB(unsigned short), at);
BITS_AtLoopBenchmark(BOOST_DB(unsigned), at);
BITS_AtLoopBenchmark(BOOST_CONST_DB(unsigned), at);
BITS_AtLoopBenchmark(BOOST_DB(unsigned long), at);
BITS_AtLoopBenchmark(BOOST_CONST_DB(unsigned long), at);
BITS_AtLoopBenchmark(BOOST_DB(unsigned long long), at);
BITS_AtLoopBenchmark(BOOST_CONST_DB(unsigned long long), at);

BITS_TestLoopBenchmark(BOOST_DB(unsigned char), test);
BITS_TestLoopBenchmark(BOOST_CONST_DB(unsigned char), test);
BITS_TestLoopBenchmark(BOOST_DB(unsigned short), test);
BITS_TestLoopBenchmark(BOOST_CONST_DB(unsigned short), test);
BITS_TestLoopBenchmark(BOOST_DB(unsigned), test);
BITS_TestLoopBenchmark(BOOST_CONST_DB(unsigned), test);
BITS_TestLoopBenchmark(BOOST_DB(unsigned long), test);
BITS_TestLoopBenchmark(BOOST_CONST_DB(unsigned long), test);
BITS_TestLoopBenchmark(BOOST_DB(unsigned long long), test);
BITS_TestLoopBenchmark(BOOST_CONST_DB(unsigned long long), test);

BITS_SetBenchmark(BOOST_DB(unsigned char), set);
BITS_SetBenchmark(BOOST_DB(unsigned short), set);
BITS_SetBenchmark(BOOST_DB(unsigned), set);
BITS_SetBenchmark(BOOST_DB(unsigned long), set);
BITS_SetBenchmark(BOOST_DB(unsigned long long), set);

BITS_ResetBenchmark(BOOST_DB(unsigned char), reset);
BITS_ResetBenchmark(BOOST_DB(unsigned short), reset);
BITS_ResetBenchmark(BOOST_DB(unsigned), reset);
BITS_ResetBenchmark(BOOST_DB(unsigned long), reset);
BITS_ResetBenchmark(BOOST_DB(unsigned long long), reset);

BITS_FlipBenchmark(BOOST_DB(unsigned char), flip);
BITS_FlipBenchmark(BOOST_DB(unsigned short), flip);
BITS_FlipBenchmark(BOOST_DB(unsigned), flip);
BITS_FlipBenchmark(BOOST_DB(unsigned long), flip);
BITS_FlipBenchmark(BOOST_DB(unsigned long long), flip);

BITS_SwapBenchmark(BOOST_DB(unsigned char), swap);
BITS_SwapBenchmark(BOOST_DB(unsigned short), swap);
BITS_SwapBenchmark(BOOST_DB(unsigned), swap);
BITS_SwapBenchmark(BOOST_DB(unsigned long), swap);
BITS_SwapBenchmark(BOOST_DB(unsigned long), swap);

BITS_AnyBenchmark(BOOST_DB(unsigned char), Any);
BITS_AnyBenchmark(BOOST_DB(unsigned short), Any);
BITS_AnyBenchmark(BOOST_DB(unsigned), Any);
BITS_AnyBenchmark(BOOST_DB(unsigned long), Any);
BITS_AnyBenchmark(BOOST_DB(unsigned long long), Any);

BITS_NoneBenchmark(BOOST_DB(unsigned char), None);
BITS_NoneBenchmark(BOOST_DB(unsigned short), None);
BITS_NoneBenchmark(BOOST_DB(unsigned), None);
BITS_NoneBenchmark(BOOST_DB(unsigned long), None);
BITS_NoneBenchmark(BOOST_DB(unsigned long long), None);

BITS_SizeBenchmark(BOOST_DB(unsigned char), Size);
BITS_SizeBenchmark(BOOST_DB(unsigned short), Size);
BITS_SizeBenchmark(BOOST_DB(unsigned), Size);
BITS_SizeBenchmark(BOOST_DB(unsigned long), Size);
BITS_SizeBenchmark(BOOST_DB(unsigned long long), Size);

BITS_CapacityBenchmark(BOOST_DB(unsigned char), Capacity);
BITS_CapacityBenchmark(BOOST_DB(unsigned short), Capacity);
BITS_CapacityBenchmark(BOOST_DB(unsigned), Capacity);
BITS_CapacityBenchmark(BOOST_DB(unsigned long), Capacity);
BITS_CapacityBenchmark(BOOST_DB(unsigned long long), Capacity);

BITS_InverseBenchmark(BOOST_DB(unsigned char), operator~);
BITS_InverseBenchmark(BOOST_DB(unsigned short), operator~);
BITS_InverseBenchmark(BOOST_DB(unsigned), operator~);
BITS_InverseBenchmark(BOOST_DB(unsigned long), operator~);
BITS_InverseBenchmark(BOOST_DB(unsigned long long), operator~);

BITS_AndOperatorBenchmark(BOOST_DB(unsigned char), operator&=);
BITS_AndOperatorBenchmark(BOOST_DB(unsigned short), operator&=);
BITS_AndOperatorBenchmark(BOOST_DB(unsigned), operator&=);
BITS_AndOperatorBenchmark(BOOST_DB(unsigned long), operator&=);
BITS_AndOperatorBenchmark(BOOST_DB(unsigned long long), operator&=);

BITS_OrOperatorBenchmark(BOOST_DB(unsigned char), operator|=);
BITS_OrOperatorBenchmark(BOOST_DB(unsigned short), operator|=);
BITS_OrOperatorBenchmark(BOOST_DB(unsigned), operator|=);
BITS_OrOperatorBenchmark(BOOST_DB(unsigned long), operator|=);
BITS_OrOperatorBenchmark(BOOST_DB(unsigned long long), operator|=);

BITS_XorOperatorBenchmark(BOOST_DB(unsigned char), operator^=);
BITS_XorOperatorBenchmark(BOOST_DB(unsigned short), operator^=);
BITS_XorOperatorBenchmark(BOOST_DB(unsigned), operator^=);
BITS_XorOperatorBenchmark(BOOST_DB(unsigned long), operator^=);
BITS_XorOperatorBenchmark(BOOST_DB(unsigned long long), operator^=);

BENCHMARK_MAIN();