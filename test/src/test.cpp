#include <gtest/gtest.h>
#include <memory_resource>
#include <cstdint>
#include <dynamic_bitset/dynamic_bitset.hpp>

class VECTOR_TEST_FIXTURE
    : public ::testing::Test
{
 protected:
  bits::DynamicBitset<> emptyVector;
  bits::DynamicBitset<> filledVector{16, 0xffff};
};

TEST_F(VECTOR_TEST_FIXTURE, CONSTRUCTOR_TEST)
{
  EXPECT_EQ(0, emptyVector.Size())
  << "Empty object must be initialized with zero size";
  EXPECT_EQ(0, emptyVector.Capacity())
  << "Empty object must be initialized with zero Capacity";
  EXPECT_EQ(nullptr, emptyVector.Data())
  << "Empty object must be initialized with Empty storage";

  EXPECT_EQ(16, filledVector.Size());
  EXPECT_EQ(64, filledVector.Capacity());
  ASSERT_NE(nullptr, filledVector.Data());
  EXPECT_EQ("1111111111111111", filledVector.ToString())
  << "Vector was initialized with 0xffff which is 0b1111111111111111";
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_CONSTRUCTOR_TEST)
{
  bits::DynamicBitset<> testVector{filledVector};

  EXPECT_EQ(testVector.Size(), filledVector.Size());
  EXPECT_EQ(testVector.Capacity(), filledVector.Capacity());
  EXPECT_EQ(testVector.ToString(), filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, MOVE_CONSTRUCTOR_TEST)
{
  auto testVector(std::move(filledVector));

  EXPECT_EQ(16, testVector.Size());
  EXPECT_EQ(64, testVector.Capacity());
  ASSERT_NE(nullptr, testVector.Data());
  EXPECT_EQ("1111111111111111", testVector.ToString());

  EXPECT_EQ(emptyVector.Size(), filledVector.Size())
  << "Moved object must be Empty with zero size";
  EXPECT_EQ(emptyVector.Capacity(), filledVector.Capacity())
  << "Moved object must be Empty with zero Capacity";
  EXPECT_EQ(emptyVector.Data(), filledVector.Data())
  << "Moved object must be Empty with nullptr storage";
}

TEST_F(VECTOR_TEST_FIXTURE, SIZE_METHOD_TEST)
{
  EXPECT_EQ(0, emptyVector.Size());
  EXPECT_EQ(16, filledVector.Size());
}

TEST_F(VECTOR_TEST_FIXTURE, CAPACITY_METHOD_TEST)
{
  EXPECT_EQ(0, emptyVector.Capacity());
  EXPECT_EQ(64, filledVector.Capacity());
}

TEST_F(VECTOR_TEST_FIXTURE, DATA_METHOD_TEST)
{
  EXPECT_EQ(nullptr, emptyVector.Data());
  EXPECT_NE(nullptr, filledVector.Data());
}

TEST_F(VECTOR_TEST_FIXTURE, MAX_SIZE_METHOD_TEST)
{
  EXPECT_EQ(std::numeric_limits<typename decltype(emptyVector)::sizeType>::max(), emptyVector.MaxSize());
}

TEST_F(VECTOR_TEST_FIXTURE, COUNT_METHOD_TEST)
{
  EXPECT_EQ(0, emptyVector.Count())
  << "Count on Empty object must return zero";
  EXPECT_EQ(16, filledVector.Count());
}

TEST_F(VECTOR_TEST_FIXTURE, RESERVE_METHOD)
{
  EXPECT_THROW(
      emptyVector.Reserve(std::numeric_limits<size_t>::max()),
      std::bad_array_new_length
  );
  emptyVector.Reserve(10);

  EXPECT_EQ(0, emptyVector.Size());
  ASSERT_EQ(640, emptyVector.Capacity())
  << "Invalid reservation of space which must be: previous + new <=> 0 + 10 = 10";
  ASSERT_NE(nullptr, emptyVector.Data());

  filledVector.Reserve(10);

  EXPECT_EQ(16, filledVector.Size());
  ASSERT_EQ(704, filledVector.Capacity())
  << "Invalid reservation of space which must be: previous + new <=> 2 + 10 = 12";
  ASSERT_NE(nullptr, filledVector.Data());
}

TEST_F(VECTOR_TEST_FIXTURE, SHRINK_TO_FIT_METHOD)
{
  emptyVector.Reserve(10);
  emptyVector.ShrinkToFit();

  EXPECT_EQ(0, emptyVector.Size())
  << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(0, emptyVector.Capacity())
  << "Zero number of bits means Empty object and in this case it must clear the object";
  EXPECT_EQ(nullptr, emptyVector.Data())
  << "Nullptr must be Set after cleaning the object";

  filledVector.ShrinkToFit();

  EXPECT_EQ(16, filledVector.Size())
  << "Shrink to fit must not modify the number of bits";
  EXPECT_EQ(64, filledVector.Capacity())
  << "If object does not contain extra space it must not change object state";
  EXPECT_NE(nullptr, filledVector.Data())
  << "Nullptr must be Set if object is Empty but 'filledVector' is not";
}

TEST_F(VECTOR_TEST_FIXTURE, ANY_METHOD)
{
  EXPECT_EQ(false, emptyVector.Any())
  << "Empty object can not contain Any bits";
  EXPECT_EQ(true, filledVector.Any());
}

TEST_F(VECTOR_TEST_FIXTURE, NONE_METHOD)
{
  EXPECT_EQ(true, emptyVector.None())
  << "Empty object can not contain Any bits";
  EXPECT_EQ(false, filledVector.None());
}

TEST_F(VECTOR_TEST_FIXTURE, EMPTY_METHOD)
{
  EXPECT_EQ(true, emptyVector.Empty())
  << "Empty object can not contain Any bits";
  EXPECT_EQ(false, filledVector.Empty());
}

TEST_F(VECTOR_TEST_FIXTURE, CLEAR_METHOD)
{
  emptyVector.Clear();
  filledVector.Clear();

  EXPECT_EQ(emptyVector.Size(), filledVector.Size())
  << "Every object must be Empty after clear";
  EXPECT_EQ(emptyVector.Capacity(), filledVector.Capacity())
  << "Every object must be Empty after clear";
  EXPECT_EQ(emptyVector.Data(), filledVector.Data())
  << "Every object must be Empty after clear";
}

TEST_F(VECTOR_TEST_FIXTURE, RESIZE_METHOD)
{
  emptyVector.Resize(10, false);
  filledVector.Resize(20, true);

  EXPECT_EQ(10, emptyVector.Size())
  << "Resize must modify the number of bits";
  EXPECT_EQ(64, emptyVector.Capacity())
  << "Resize must modify the number of bytes";
  ASSERT_NE(nullptr, emptyVector.Data());
  EXPECT_EQ(true, emptyVector.None())
  << "Bits must be unset due to second parameter -> false";

  EXPECT_EQ(20, filledVector.Size());
  EXPECT_EQ(64, filledVector.Capacity());
  ASSERT_NE(nullptr, filledVector.Data());
  EXPECT_EQ(20, filledVector.Count())
  << "New bits must be Set due to second parameter -> true";
}

TEST_F(VECTOR_TEST_FIXTURE, PUSH_BACK_METHOD)
{
  for (std::size_t i{}; i < 10; ++i)
  {
    emptyVector.PushBack(true);
    filledVector.PushBack(true);
  }

  EXPECT_EQ(10, emptyVector.Count());
  EXPECT_EQ(26, filledVector.Count());
}

TEST_F(VECTOR_TEST_FIXTURE, POP_BACK_METHOD)
{
  for (std::size_t i {}; i < 16; ++i)
  {
    filledVector.PopBack();
  }
  EXPECT_EQ(0, filledVector.Size());
}

TEST_F(VECTOR_TEST_FIXTURE, SET_INDEX_METHOD)
{
  EXPECT_THROW(emptyVector.Set(10), std::out_of_range);
  
  for (std::size_t i{}; i < 4; ++i)
  {
    filledVector.Set(i, false);
  }
  
  EXPECT_EQ("0000111111111111", filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, SET_METHOD)
{
  EXPECT_THROW(emptyVector.Set(), std::out_of_range);

  bits::DynamicBitset<> testVector{16};
  testVector.Set();

  EXPECT_EQ(false, testVector.None())
  << "Set method must Set all bits to true";
}

TEST_F(VECTOR_TEST_FIXTURE, RESET_INDEX_METHOD)
{
  EXPECT_THROW(emptyVector.Reset(10), std::out_of_range);

  for (std::size_t i{}; i < 4; ++i)
  {
    filledVector.Reset(i);
  }
  
  EXPECT_EQ("0000111111111111", filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, RESET_METHOD)
{
  EXPECT_THROW(emptyVector.Reset(), std::out_of_range);

  filledVector.Reset();

  EXPECT_EQ(true, filledVector.None())
  << "Reset method must Set all bits to false";
}

TEST_F(VECTOR_TEST_FIXTURE, FLIP_INDEX_METHOD)
{
  EXPECT_THROW(emptyVector.Flip(10), std::out_of_range);

  for (std::size_t i{}; i < 4; ++i)
  {
    filledVector.Flip(i);
  }
  
  EXPECT_EQ("0000111111111111", filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, FLIP_METHOD)
{
  EXPECT_THROW(emptyVector.Flip(), std::out_of_range);

  filledVector.Flip();

  EXPECT_EQ(true, filledVector.None());
}

TEST_F(VECTOR_TEST_FIXTURE, SWAP_METHOD)
{
  emptyVector.Swap(filledVector);

  EXPECT_EQ(16, emptyVector.Size());
  EXPECT_EQ(64, emptyVector.Capacity());
  EXPECT_EQ(16, emptyVector.Count());
  EXPECT_EQ(0, filledVector.Size());
  EXPECT_EQ(0, filledVector.Capacity());
  EXPECT_EQ(nullptr, filledVector.Data());
}

TEST_F(VECTOR_TEST_FIXTURE, SUBSCRIPT_OPERATOR_TEST)
{
  for (std::size_t i{}; i < 10; ++i)
  {
    ASSERT_EQ(true, filledVector[i]);
    filledVector[i] = false;
    ASSERT_EQ(false, filledVector[i]);
    filledVector[i] = true;
    ASSERT_EQ(true, filledVector[i]);
  }
}

TEST_F(VECTOR_TEST_FIXTURE, AT_METHOD)
{
  EXPECT_THROW((void)emptyVector.At(10), std::out_of_range);
  EXPECT_THROW((void)filledVector.At(20), std::out_of_range);

  EXPECT_EQ(true, filledVector.At(0));
  filledVector.Reset(0);
  EXPECT_EQ(false, filledVector.At(0));
}

TEST_F(VECTOR_TEST_FIXTURE, FRONT_METHOD)
{
  EXPECT_EQ(true, filledVector.Front());
  filledVector.Reset(0);
  EXPECT_EQ(false, filledVector.Front());
}

TEST_F(VECTOR_TEST_FIXTURE, BACK_METHOD)
{
  EXPECT_EQ(true, filledVector.Back());
  filledVector.Reset(15);
  EXPECT_EQ(false, filledVector.Back());
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_ASSIGNMENT_OPERATOR)
{
  emptyVector = filledVector;

  EXPECT_EQ(emptyVector.Size(), filledVector.Size());
  EXPECT_EQ(emptyVector.Capacity(), filledVector.Capacity());
  ASSERT_NE(nullptr, emptyVector.Data());
  EXPECT_EQ(emptyVector.ToString(), filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, MOVE_ASSIGNMENT_OPERATOR)
{
  emptyVector = std::move(filledVector);

  EXPECT_EQ(16, emptyVector.Size());
  EXPECT_EQ(64, emptyVector.Capacity());
  EXPECT_EQ(16, emptyVector.Count());
  EXPECT_EQ(0, filledVector.Size());
  EXPECT_EQ(0, filledVector.Capacity());
  EXPECT_EQ(nullptr, filledVector.Data());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_AND_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(emptyVector &= filledVector, std::invalid_argument);

  emptyVector.Resize(16, true);
  filledVector &= emptyVector;

  ASSERT_EQ(16, filledVector.Size());
  ASSERT_EQ(64, filledVector.Capacity());
  ASSERT_EQ(16, filledVector.Count());

  emptyVector.Reset();
  filledVector &= emptyVector;

  EXPECT_EQ(16, filledVector.Size());
  EXPECT_EQ(64, filledVector.Capacity());
  EXPECT_EQ(true, filledVector.None());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_OR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(emptyVector |= filledVector, std::invalid_argument);

  emptyVector.Resize(16, true);
  filledVector |= emptyVector;

  ASSERT_EQ(16, filledVector.Size());
  ASSERT_EQ(64, filledVector.Capacity());
  ASSERT_EQ(16, filledVector.Count());

  emptyVector.Reset();
  filledVector |= emptyVector;

  EXPECT_EQ(16, filledVector.Size());
  EXPECT_EQ(64, filledVector.Capacity());
  EXPECT_EQ(false, filledVector.None());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_XOR_ASSIGNMENT_OPERATOR_TEST)
{
  EXPECT_THROW(emptyVector ^= filledVector, std::invalid_argument);

  emptyVector.Resize(16, true);
  filledVector ^= emptyVector;

  ASSERT_EQ(16, filledVector.Size());
  ASSERT_EQ(64, filledVector.Capacity());
  ASSERT_EQ(true, filledVector.None());

  filledVector ^= emptyVector;

  EXPECT_EQ(16, filledVector.Size());
  EXPECT_EQ(64, filledVector.Capacity());
  EXPECT_EQ(16, filledVector.Count());
}

TEST_F(VECTOR_TEST_FIXTURE, BITWISE_INVERSE_OPERATOR_TEST)
{
  EXPECT_THROW((void)~emptyVector, std::out_of_range);

  emptyVector = ~filledVector;

  ASSERT_EQ(16, emptyVector.Size());
  ASSERT_EQ(64, emptyVector.Capacity());
  ASSERT_EQ(true, emptyVector.None());
}

TEST_F(VECTOR_TEST_FIXTURE, TO_STRING_METHOD_TEST)
{
  EXPECT_EQ("", emptyVector.ToString());
  EXPECT_EQ("1111111111111111", filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, LHS_ASSIGNMENT_TEST)
{
  EXPECT_THROW(emptyVector >>= 10, std::out_of_range);

  filledVector >>= 0;

  ASSERT_EQ(16, filledVector.Size());
  ASSERT_EQ(64, filledVector.Capacity());
  ASSERT_EQ(16, filledVector.Count());

  filledVector >>= 8;

  ASSERT_EQ("0000000011111111", filledVector.ToString());

  filledVector >>= 8;

  ASSERT_EQ(true, filledVector.None());

  filledVector.Set();
  filledVector >>= 20;

  ASSERT_EQ(true, filledVector.None());

  filledVector.Set();
  filledVector >>= 5;

  EXPECT_EQ("0000011111111111", filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, RHS_ASSIGNMENT_TEST)
{
  EXPECT_THROW(emptyVector <<= 10, std::out_of_range);

  filledVector <<= 0;

  ASSERT_EQ(16, filledVector.Size());
  ASSERT_EQ(64, filledVector.Capacity());
  ASSERT_EQ(16, filledVector.Count());

  filledVector <<= 8;

  ASSERT_EQ("1111111100000000", filledVector.ToString());

  filledVector <<= 8;

  ASSERT_EQ(true, filledVector.None());

  filledVector.Set();
  filledVector <<= 20;

  ASSERT_EQ(true, filledVector.None());
  filledVector.Set();
  filledVector <<= 5;

  EXPECT_EQ("1111111111100000", filledVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_LHS_TEST)
{
  emptyVector = filledVector << 8;

  ASSERT_EQ("1111111100000000", emptyVector.ToString());

  emptyVector = filledVector << 1;

  EXPECT_EQ("1111111111111110", emptyVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, COPY_RHS_TEST)
{
  emptyVector = filledVector >> 8;

  ASSERT_EQ("0000000011111111", emptyVector.ToString());

  emptyVector = filledVector >> 1;

  EXPECT_EQ("0111111111111111", emptyVector.ToString());
}

TEST_F(VECTOR_TEST_FIXTURE, ADVANCED_LHS_RHS_TEST)
{
  for (std::size_t i{}; i < 16; ++i)
  {
    filledVector >>= 1;
    ASSERT_EQ(false, filledVector[i]);
  }

  filledVector.Set();

  for (std::size_t i{}; i < 16; ++i)
  {
    filledVector <<= 1;
    ASSERT_EQ(false, filledVector[15 - i]);
  }

  filledVector.Set();
  filledVector >>= 8;

  ASSERT_EQ("0000000011111111", filledVector.ToString());

  filledVector >>= 3;

  ASSERT_EQ("0000000000011111", filledVector.ToString());

  filledVector >>= 5;

  ASSERT_EQ(true, filledVector.None());

  filledVector.Set();
  filledVector <<= 8;

  ASSERT_EQ("1111111100000000", filledVector.ToString());

  filledVector <<= 3;

  ASSERT_EQ("1111100000000000", filledVector.ToString());

  filledVector <<= 5;

  EXPECT_EQ(true, filledVector.None());
}

TEST_F(VECTOR_TEST_FIXTURE, ITERATOR_TEST)
{
  for (auto&& iter : filledVector)
  {
    ASSERT_EQ(true, iter);
    iter = false;
    ASSERT_EQ(false, iter);
  }

  for (const auto& iter : filledVector)
    ASSERT_EQ(false, iter);

  for ([[maybe_unused]] auto& iter : emptyVector)
    ASSERT_EQ(false, true)
    << "Error in for range loop traversing Empty object";
  
  for ([[maybe_unused]] const auto& iter : emptyVector)
    ASSERT_EQ(false, true)
    << "Error in for range loop (const) traversing Empty object";
}

TEST_F(VECTOR_TEST_FIXTURE, STRESS_TEST)
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

std::size_t buffer[1'000];

TEST_F(VECTOR_TEST_FIXTURE, TEMPLATE_ALLOCATOR_TEST)
{
  std::pmr::monotonic_buffer_resource rs{static_cast<void*>(buffer), 1'000};
  std::pmr::polymorphic_allocator<std::size_t> pAlloc{&rs};
  bits::DynamicBitset<typename std::allocator_traits<decltype(pAlloc)>::value_type, decltype(pAlloc)> testVector{pAlloc};
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
