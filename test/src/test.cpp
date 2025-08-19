#include <gtest/gtest.h>

#include <cstdint>
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <memory_resource>

class DynamicBitsetFixture : public testing::Test
{
 protected:
  bits::DynamicBitset<> empty_bitset;
  bits::DynamicBitset<> filled_bitset{16, 0xff'ff};
};

TEST_F(
  DynamicBitsetFixture,  //
  CONSTRUCTOR_TEST
)
{
  EXPECT_EQ(0, empty_bitset.Size()) << "empty object must be initialized with zero size";
  EXPECT_EQ(0, empty_bitset.Capacity()) << "empty object must be initialized with zero capacity";
  EXPECT_EQ(nullptr, empty_bitset.Data()) << "empty object must be initialized with empty storage";

  EXPECT_EQ(16, filled_bitset.Size());
  EXPECT_EQ(64, filled_bitset.Capacity());
  ASSERT_NE(nullptr, filled_bitset.Data());
  EXPECT_EQ("1111111111111111", filled_bitset.ToString())
    << "Vector was initialized with 0xffff which is 0b1111111111111111";
}

TEST_F(
  DynamicBitsetFixture,  //
  COPY_CONSTRUCTOR_TEST
)
{
  bits::DynamicBitset<> testVector{filled_bitset};

  EXPECT_EQ(testVector.Size(), filled_bitset.Size());
  EXPECT_EQ(testVector.Capacity(), filled_bitset.Capacity());
  EXPECT_EQ(testVector.ToString(), filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  MOVE_CONSTRUCTOR_TEST
)
{
  auto testVector(std::move(filled_bitset));

  EXPECT_EQ(16, testVector.Size());
  EXPECT_EQ(64, testVector.Capacity());
  ASSERT_NE(nullptr, testVector.Data());
  EXPECT_EQ("1111111111111111", testVector.ToString());

  EXPECT_EQ(empty_bitset.Size(), filled_bitset.Size()) << "Moved object must be empty with zero size";
  EXPECT_EQ(empty_bitset.Capacity(), filled_bitset.Capacity()) << "Moved object must be empty with zero capacity";
  EXPECT_EQ(empty_bitset.Data(), filled_bitset.Data()) << "Moved object must be empty with nullptr storage";
}

TEST_F(
  DynamicBitsetFixture,  //
  SIZE_METHOD_TEST
)
{
  EXPECT_EQ(0, empty_bitset.Size());
  EXPECT_EQ(16, filled_bitset.Size());
}

TEST_F(
  DynamicBitsetFixture,  //
  CAPACITY_METHOD_TEST
)
{
  EXPECT_EQ(0, empty_bitset.Capacity());
  EXPECT_EQ(64, filled_bitset.Capacity());
}

TEST_F(
  DynamicBitsetFixture,  //
  DATA_METHOD_TEST
)
{
  EXPECT_EQ(nullptr, empty_bitset.Data());
  EXPECT_NE(nullptr, filled_bitset.Data());
}

TEST_F(
  DynamicBitsetFixture,  //
  MAX_SIZE_METHOD_TEST
)
{
  EXPECT_EQ(std::numeric_limits<typename decltype(empty_bitset)::SizeType>::max(), empty_bitset.MaxSize());
}

TEST_F(
  DynamicBitsetFixture,  //
  COUNT_METHOD_TEST
)
{
  EXPECT_EQ(0, empty_bitset.Count()) << "count on empty object must return zero";
  EXPECT_EQ(16, filled_bitset.Count());
}

TEST_F(
  DynamicBitsetFixture,  //
  RESERVE_METHOD
)
{
  EXPECT_THROW(empty_bitset.Reserve(std::numeric_limits<size_t>::max()), std::bad_array_new_length);
  empty_bitset.Reserve(10);

  EXPECT_EQ(0, empty_bitset.Size());
  ASSERT_EQ(640, empty_bitset.Capacity())
    << "Invalid reservation of space which must be: previous + new <=> 0 + 10 = 10";
  ASSERT_NE(nullptr, empty_bitset.Data());

  filled_bitset.Reserve(10);

  EXPECT_EQ(16, filled_bitset.Size());
  ASSERT_EQ(704, filled_bitset.Capacity())
    << "Invalid reservation of space which must be: previous + new <=> 2 + 10 = 12";
  ASSERT_NE(nullptr, filled_bitset.Data());
}

TEST_F(
  DynamicBitsetFixture,  //
  SHRINK_TO_FIT_METHOD
)
{
  empty_bitset.Reserve(10);
  empty_bitset.ShrinkToFit();

  EXPECT_EQ(0, empty_bitset.Size()) << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(
    0, empty_bitset.Capacity()
  ) << "Zero number of bits means empty object and in this case it must clear the object";
  EXPECT_EQ(nullptr, empty_bitset.Data()) << "Nullptr must be set after cleaning the object";

  filled_bitset.ShrinkToFit();

  EXPECT_EQ(16, filled_bitset.Size()) << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(64, filled_bitset.Capacity()) << "If object does not contain extra space it must not change object state";
  EXPECT_NE(nullptr, filled_bitset.Data()) << "Nullptr must be set if object is empty but 'filled_bitset' is not";
}

TEST_F(
  DynamicBitsetFixture,  //
  ALL_METHOD
)
{
  EXPECT_EQ(false, empty_bitset.All()) << "empty object can not containe any bits";
  EXPECT_EQ(true, filled_bitset.All());
}

TEST_F(
  DynamicBitsetFixture,  //
  ANY_METHOD
)
{
  EXPECT_EQ(false, empty_bitset.Any()) << "empty object can not contain any bits";
  EXPECT_EQ(true, filled_bitset.Any());
}

TEST_F(
  DynamicBitsetFixture,  //
  NONE_METHOD
)
{
  EXPECT_EQ(true, empty_bitset.None()) << "empty object can not contain any bits";
  EXPECT_EQ(false, filled_bitset.None());
}

TEST_F(
  DynamicBitsetFixture,  //
  EMPTY_METHOD
)
{
  EXPECT_EQ(true, empty_bitset.Empty()) << "empty object can not contain any bits";
  EXPECT_EQ(false, filled_bitset.Empty());
}

TEST_F(
  DynamicBitsetFixture,  //
  CLEAR_METHOD
)
{
  empty_bitset.Clear();
  filled_bitset.Clear();

  EXPECT_EQ(empty_bitset.Size(), filled_bitset.Size()) << "Every object must be empty after clear";
  EXPECT_EQ(empty_bitset.Capacity(), filled_bitset.Capacity()) << "Every object must be empty after clear";
  EXPECT_EQ(empty_bitset.Data(), filled_bitset.Data()) << "Every object must be empty after clear";
}

TEST_F(
  DynamicBitsetFixture,  //
  RESIZE_METHOD
)
{
  empty_bitset.Resize(10, false);
  filled_bitset.Resize(20, true);

  EXPECT_EQ(10, empty_bitset.Size()) << "'Resize' must modify the number of bits";
  EXPECT_EQ(64, empty_bitset.Capacity()) << "'Resize' must modify the number of bytes";
  ASSERT_NE(nullptr, empty_bitset.Data());
  EXPECT_EQ(true, empty_bitset.None()) << "Bits must be unset due to second parameter -> false";

  EXPECT_EQ(20, filled_bitset.Size());
  EXPECT_EQ(64, filled_bitset.Capacity());
  ASSERT_NE(nullptr, filled_bitset.Data());
  EXPECT_EQ(20, filled_bitset.Count()) << "New bits must be set due to second parameter -> true";
}

TEST_F(
  DynamicBitsetFixture,  //
  PUSH_BACK_METHOD
)
{
  for (std::size_t i{}; i < 10; ++i)
  {
    empty_bitset.PushBack(true);
    filled_bitset.PushBack(true);
  }

  EXPECT_EQ(10, empty_bitset.Count());
  EXPECT_EQ(26, filled_bitset.Count());
}

TEST_F(
  DynamicBitsetFixture,  //
  POP_BACK_METHOD
)
{
  for (std::size_t i{}; i < 16; ++i)
  {
    filled_bitset.PopBack();
  }
  EXPECT_EQ(0, filled_bitset.Size());
}

TEST_F(
  DynamicBitsetFixture,  //
  SET_INDEX_METHOD
)
{
  EXPECT_THROW(empty_bitset.Set(10), std::out_of_range);

  for (std::size_t i{}; i < 4; ++i)
  {
    filled_bitset.Set(i, false);
  }

  EXPECT_EQ("0000111111111111", filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  SET_METHOD
)
{
  EXPECT_THROW(empty_bitset.Set(), std::out_of_range);

  bits::DynamicBitset<> testVector{16};
  testVector.Set();

  EXPECT_EQ(false, testVector.None()) << "'Set' method must set all bits to true";
}

TEST_F(
  DynamicBitsetFixture,  //
  RESET_INDEX_METHOD
)
{
  EXPECT_THROW(empty_bitset.Reset(10), std::out_of_range);

  for (std::size_t i{}; i < 4; ++i)
  {
    filled_bitset.Reset(i);
  }

  EXPECT_EQ("0000111111111111", filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  RESET_METHOD
)
{
  EXPECT_THROW(empty_bitset.Reset(), std::out_of_range);

  filled_bitset.Reset();

  EXPECT_EQ(true, filled_bitset.None()) << "Reset method must set all bits to false";
}

TEST_F(
  DynamicBitsetFixture,  //
  FLIP_INDEX_METHOD
)
{
  EXPECT_THROW(empty_bitset.Flip(10), std::out_of_range);

  for (std::size_t i{}; i < 4; ++i)
  {
    filled_bitset.Flip(i);
  }

  EXPECT_EQ("0000111111111111", filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  FLIP_METHOD
)
{
  EXPECT_THROW(empty_bitset.Flip(), std::out_of_range);

  filled_bitset.Flip();

  EXPECT_EQ(true, filled_bitset.None());
}

TEST_F(
  DynamicBitsetFixture,  //
  SWAP_METHOD
)
{
  empty_bitset.Swap(filled_bitset);

  EXPECT_EQ(16, empty_bitset.Size());
  EXPECT_EQ(64, empty_bitset.Capacity());
  EXPECT_EQ(16, empty_bitset.Count());
  EXPECT_EQ(0, filled_bitset.Size());
  EXPECT_EQ(0, filled_bitset.Capacity());
  EXPECT_EQ(nullptr, filled_bitset.Data());
}

TEST_F(
  DynamicBitsetFixture,  //
  SUBSCRIPT_OPERATOR_TEST
)
{
  for (std::size_t i{}; i < 10; ++i)
  {
    ASSERT_EQ(true, filled_bitset[i]);
    filled_bitset[i] = false;
    ASSERT_EQ(false, filled_bitset[i]);
    filled_bitset[i] = true;
    ASSERT_EQ(true, filled_bitset[i]);
  }
}

TEST_F(
  DynamicBitsetFixture,  //
  AT_METHOD
)
{
  EXPECT_THROW((void) empty_bitset.At(10), std::out_of_range);
  EXPECT_THROW((void) filled_bitset.At(20), std::out_of_range);

  EXPECT_EQ(true, filled_bitset.At(0));
  filled_bitset.Reset(0);
  EXPECT_EQ(false, filled_bitset.At(0));
}

TEST_F(
  DynamicBitsetFixture,  //
  FRONT_METHOD
)
{
  EXPECT_EQ(true, filled_bitset.Front());
  filled_bitset.Reset(0);
  EXPECT_EQ(false, filled_bitset.Front());
}

TEST_F(
  DynamicBitsetFixture,  //
  BACK_METHOD
)
{
  EXPECT_EQ(true, filled_bitset.Back());
  filled_bitset.Reset(15);
  EXPECT_EQ(false, filled_bitset.Back());
}

TEST_F(
  DynamicBitsetFixture,  //
  COPY_ASSIGNMENT_OPERATOR
)
{
  empty_bitset = filled_bitset;

  EXPECT_EQ(empty_bitset.Size(), filled_bitset.Size());
  EXPECT_EQ(empty_bitset.Capacity(), filled_bitset.Capacity());
  ASSERT_NE(nullptr, empty_bitset.Data());
  EXPECT_EQ(empty_bitset.ToString(), filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  MOVE_ASSIGNMENT_OPERATOR
)
{
  empty_bitset = std::move(filled_bitset);

  EXPECT_EQ(16, empty_bitset.Size());
  EXPECT_EQ(64, empty_bitset.Capacity());
  EXPECT_EQ(16, empty_bitset.Count());
  EXPECT_EQ(0, filled_bitset.Size());
  EXPECT_EQ(0, filled_bitset.Capacity());
  EXPECT_EQ(nullptr, filled_bitset.Data());
}

TEST_F(
  DynamicBitsetFixture,  //
  BITWISE_AND_ASSIGNMENT_OPERATOR_TEST
)
{
  EXPECT_THROW(empty_bitset &= filled_bitset, std::invalid_argument);

  empty_bitset.Resize(16, true);
  filled_bitset &= empty_bitset;

  ASSERT_EQ(16, filled_bitset.Size());
  ASSERT_EQ(64, filled_bitset.Capacity());
  ASSERT_EQ(16, filled_bitset.Count());

  empty_bitset.Reset();
  filled_bitset &= empty_bitset;

  EXPECT_EQ(16, filled_bitset.Size());
  EXPECT_EQ(64, filled_bitset.Capacity());
  EXPECT_EQ(true, filled_bitset.None());
}

TEST_F(
  DynamicBitsetFixture,  //
  BITWISE_OR_ASSIGNMENT_OPERATOR_TEST
)
{
  EXPECT_THROW(empty_bitset |= filled_bitset, std::invalid_argument);

  empty_bitset.Resize(16, true);
  filled_bitset |= empty_bitset;

  ASSERT_EQ(16, filled_bitset.Size());
  ASSERT_EQ(64, filled_bitset.Capacity());
  ASSERT_EQ(16, filled_bitset.Count());

  empty_bitset.Reset();
  filled_bitset |= empty_bitset;

  EXPECT_EQ(16, filled_bitset.Size());
  EXPECT_EQ(64, filled_bitset.Capacity());
  EXPECT_EQ(false, filled_bitset.None());
}

TEST_F(
  DynamicBitsetFixture,  //
  BITWISE_XOR_ASSIGNMENT_OPERATOR_TEST
)
{
  EXPECT_THROW(empty_bitset ^= filled_bitset, std::invalid_argument);

  empty_bitset.Resize(16, true);
  filled_bitset ^= empty_bitset;

  ASSERT_EQ(16, filled_bitset.Size());
  ASSERT_EQ(64, filled_bitset.Capacity());
  ASSERT_EQ(true, filled_bitset.None());

  filled_bitset ^= empty_bitset;

  EXPECT_EQ(16, filled_bitset.Size());
  EXPECT_EQ(64, filled_bitset.Capacity());
  EXPECT_EQ(16, filled_bitset.Count());
}

TEST_F(
  DynamicBitsetFixture,  //
  BITWISE_INVERSE_OPERATOR_TEST
)
{
  EXPECT_THROW((void) ~empty_bitset, std::out_of_range);

  empty_bitset = ~filled_bitset;

  ASSERT_EQ(16, empty_bitset.Size());
  ASSERT_EQ(64, empty_bitset.Capacity());
  ASSERT_EQ(true, empty_bitset.None());
}

TEST_F(
  DynamicBitsetFixture,  //
  TO_STRING_METHOD_TEST
)
{
  EXPECT_EQ("", empty_bitset.ToString());
  EXPECT_EQ("1111111111111111", filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  LHS_ASSIGNMENT_TEST
)
{
  EXPECT_THROW(empty_bitset >>= 10, std::out_of_range);

  filled_bitset >>= 0;

  ASSERT_EQ(16, filled_bitset.Size());
  ASSERT_EQ(64, filled_bitset.Capacity());
  ASSERT_EQ(16, filled_bitset.Count());

  filled_bitset >>= 8;

  ASSERT_EQ("0000000011111111", filled_bitset.ToString());

  filled_bitset >>= 8;

  ASSERT_EQ(true, filled_bitset.None());

  filled_bitset.Set();
  filled_bitset >>= 20;

  ASSERT_EQ(true, filled_bitset.None());

  filled_bitset.Set();
  filled_bitset >>= 5;

  EXPECT_EQ("0000011111111111", filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  RHS_ASSIGNMENT_TEST
)
{
  EXPECT_THROW(empty_bitset <<= 10, std::out_of_range);

  filled_bitset <<= 0;

  ASSERT_EQ(16, filled_bitset.Size());
  ASSERT_EQ(64, filled_bitset.Capacity());
  ASSERT_EQ(16, filled_bitset.Count());

  filled_bitset <<= 8;

  ASSERT_EQ("1111111100000000", filled_bitset.ToString());

  filled_bitset <<= 8;

  ASSERT_EQ(true, filled_bitset.None());

  filled_bitset.Set();
  filled_bitset <<= 20;

  ASSERT_EQ(true, filled_bitset.None());
  filled_bitset.Set();
  filled_bitset <<= 5;

  EXPECT_EQ("1111111111100000", filled_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  COPY_LHS_TEST
)
{
  empty_bitset = filled_bitset << 8;

  ASSERT_EQ("1111111100000000", empty_bitset.ToString());

  empty_bitset = filled_bitset << 1;

  EXPECT_EQ("1111111111111110", empty_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture, COPY_RHS_TEST
)
{
  empty_bitset = filled_bitset >> 8;

  ASSERT_EQ("0000000011111111", empty_bitset.ToString());

  empty_bitset = filled_bitset >> 1;

  EXPECT_EQ("0111111111111111", empty_bitset.ToString());
}

TEST_F(
  DynamicBitsetFixture,  //
  ADVANCED_LHS_RHS_TEST
)
{
  for (std::size_t i{}; i < 16; ++i)
  {
    filled_bitset >>= 1;
    ASSERT_EQ(false, filled_bitset[i]);
  }

  filled_bitset.Set();

  for (std::size_t i{}; i < 16; ++i)
  {
    filled_bitset <<= 1;
    ASSERT_EQ(false, filled_bitset[15 - i]);
  }

  filled_bitset.Set();
  filled_bitset >>= 8;

  ASSERT_EQ("0000000011111111", filled_bitset.ToString());

  filled_bitset >>= 3;

  ASSERT_EQ("0000000000011111", filled_bitset.ToString());

  filled_bitset >>= 5;

  ASSERT_EQ(true, filled_bitset.None());

  filled_bitset.Set();
  filled_bitset <<= 8;

  ASSERT_EQ("1111111100000000", filled_bitset.ToString());

  filled_bitset <<= 3;

  ASSERT_EQ("1111100000000000", filled_bitset.ToString());

  filled_bitset <<= 5;

  EXPECT_EQ(true, filled_bitset.None());
}

TEST_F(
  DynamicBitsetFixture,  //
  ITERATOR_PREDICATE_TEST
)
{
  auto begin_iterator{filled_bitset.begin()};
  auto end_iterator{filled_bitset.end()};

  EXPECT_EQ(false, begin_iterator == end_iterator);
  EXPECT_EQ(true, begin_iterator != end_iterator);
  EXPECT_EQ(true, begin_iterator <= end_iterator);
  EXPECT_EQ(true, begin_iterator < end_iterator);
  EXPECT_EQ(false, begin_iterator >= end_iterator);
  EXPECT_EQ(false, begin_iterator > end_iterator);
}

TEST_F(
  DynamicBitsetFixture, //
  ITERATOR_ARITHMETIC_TEST
)
{
  auto begin_iterator{filled_bitset.begin()};
  auto end_iterator{filled_bitset.end()};
  typename decltype(filled_bitset)::SizeType bitset_size{filled_bitset.Size()};

  EXPECT_EQ(bitset_size, end_iterator - begin_iterator);
  ASSERT_EQ(false, begin_iterator + 0 == end_iterator);
  EXPECT_EQ(true, begin_iterator + bitset_size == end_iterator);
  ASSERT_EQ(true, begin_iterator + 0 != end_iterator);
  EXPECT_EQ(false, begin_iterator + bitset_size != end_iterator);
}

TEST_F(
  DynamicBitsetFixture, //
  ITERATOR_BITWISE_TEST
)
{
  ASSERT_EQ(true, filled_bitset.begin() != filled_bitset.end());
  for (auto& bit : filled_bitset)
  {
    ASSERT_EQ(true, bit);
    bit |= false;
    ASSERT_EQ(true, bit);
    bit |= true;
    ASSERT_EQ(true, bit);
    bit &= true;
    ASSERT_EQ(true, bit);
    bit &= false;
    ASSERT_EQ(false, bit);
    bit ^= true;
    ASSERT_EQ(true, bit);
    bit ^= false;
    ASSERT_EQ(true, bit);
  }
}

TEST_F(
  DynamicBitsetFixture,  //
  STRESS_TEST
)
{
  bits::DynamicBitset<> testVector;

  constexpr std::size_t SIZE{static_cast<size_t>(std::numeric_limits<int>::max())};

  for (std::size_t i{}; i < SIZE; ++i)
  {
    testVector.PushBack(true);
  }

  ASSERT_EQ(SIZE, testVector.Count());

  for (std::size_t i{}; i < SIZE; ++i)
  {
    testVector.PopBack();
  }

  ASSERT_EQ(0, testVector.Size());

  testVector.ShrinkToFit();

  constexpr std::size_t MID_SIZE{SIZE >> 1};

  testVector.Resize(SIZE, true);

  testVector >>= MID_SIZE;

  ASSERT_EQ(MID_SIZE + 1, testVector.Count());

  testVector <<= MID_SIZE;

  ASSERT_EQ(MID_SIZE + 1, testVector.Count());

  testVector.Clear();
}

std::size_t buffer[1000];

TEST_F(
  DynamicBitsetFixture,  //
  TEMPLATE_ALLOCATOR_TEST
)
{
  std::pmr::monotonic_buffer_resource rs{static_cast<void*>(buffer), 1000};
  std::pmr::polymorphic_allocator<std::size_t> pAlloc{&rs};
  bits::DynamicBitset<typename std::allocator_traits<decltype(pAlloc)>::value_type, decltype(pAlloc)> testVector{
    pAlloc
  };
  constexpr std::size_t SIZE{7200};

  for (std::size_t i{}; i < SIZE; ++i)
  {
    testVector.PushBack(true);
  }

  ASSERT_EQ(SIZE, testVector.Count());

  for (std::size_t i{}; i < SIZE; ++i)
  {
    testVector.PopBack();
  }

  ASSERT_EQ(true, testVector.None());

  testVector.ShrinkToFit();

  constexpr std::size_t MID_SIZE{SIZE >> 1};

  testVector.Resize(SIZE, true);

  testVector >>= MID_SIZE;

  ASSERT_EQ(MID_SIZE, testVector.Count());

  testVector <<= MID_SIZE;

  ASSERT_EQ(MID_SIZE, testVector.Count());

  testVector.Clear();
}
