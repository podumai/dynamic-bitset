#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <memory_resource>

class DynamicBitsetFixture : public testing::Test {
 protected:
  bits::DynamicBitset<> empty_bitset;
  bits::DynamicBitset<> filled_bitset{16, 0xff'ff};
};

TEST_F(DynamicBitsetFixture, ConstructorTest) {
  EXPECT_EQ(0, empty_bitset.Size()) << "empty object must be initialized with zero size";
  EXPECT_EQ(0, empty_bitset.Capacity()) << "empty object must be initialized with zero capacity";
  EXPECT_EQ(nullptr, empty_bitset.Data()) << "empty object must be initialized with empty storage";

  EXPECT_EQ(16, filled_bitset.Size());
  EXPECT_EQ(64, filled_bitset.Capacity());
  ASSERT_NE(nullptr, filled_bitset.Data());
  EXPECT_EQ("1111111111111111", filled_bitset.ToString())
    << "Vector was initialized with 0xffff which is 0b1111111111111111";
}

TEST_F(DynamicBitsetFixture, CopyConstructorTest) {
  auto test_vector = filled_bitset;

  EXPECT_EQ(test_vector.Size(), filled_bitset.Size());
  EXPECT_EQ(test_vector.Capacity(), filled_bitset.Capacity());
  EXPECT_EQ(test_vector.ToString(), filled_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, MoveConstructorTest) {
  auto test_vector = std::move(filled_bitset);

  EXPECT_EQ(16, test_vector.Size());
  EXPECT_EQ(64, test_vector.Capacity());
  ASSERT_NE(nullptr, test_vector.Data());
  EXPECT_EQ("1111111111111111", test_vector.ToString());

  EXPECT_EQ(empty_bitset.Size(), filled_bitset.Size()) << "Moved object must be empty with zero size";
  EXPECT_EQ(empty_bitset.Capacity(), filled_bitset.Capacity()) << "Moved object must be empty with zero capacity";
  EXPECT_EQ(empty_bitset.Data(), filled_bitset.Data()) << "Moved object must be empty with nullptr storage";
}

TEST_F(DynamicBitsetFixture, IteratorConstructorTest) {
  using BlockType = typename decltype(filled_bitset)::BlockType;
  {
    auto test_bitset = bits::DynamicBitset<BlockType>{filled_bitset.cbegin(), filled_bitset.cend()};
    EXPECT_EQ(test_bitset, filled_bitset);
  }
  {
    auto test_bitset = bits::DynamicBitset<BlockType>{empty_bitset.cbegin(), empty_bitset.cend()};
    EXPECT_EQ(test_bitset, empty_bitset);
  }
}

TEST_F(DynamicBitsetFixture, SizeMethodTest) {
  EXPECT_EQ(0, empty_bitset.Size());
  EXPECT_EQ(16, filled_bitset.Size());
}

TEST_F(DynamicBitsetFixture, CapacityMethodTest) {
  EXPECT_EQ(0, empty_bitset.Capacity());
  EXPECT_EQ(64, filled_bitset.Capacity());
}

TEST_F(DynamicBitsetFixture, NumBlocksMethodTest) {
  EXPECT_EQ(0, empty_bitset.NumBlocks());
  EXPECT_EQ(1, filled_bitset.NumBlocks());
}

TEST_F(DynamicBitsetFixture, DataMethodTest) {
  EXPECT_EQ(nullptr, empty_bitset.Data());
  EXPECT_NE(nullptr, filled_bitset.Data());
}

TEST_F(DynamicBitsetFixture, MaxSizeMethodTest) {
  EXPECT_EQ(
    std::allocator_traits<decltype(empty_bitset.GetAllocator())>::max_size(empty_bitset.GetAllocator()),
    empty_bitset.MaxSize()
  );
}

TEST_F(DynamicBitsetFixture, CountMethodTest) {
  EXPECT_EQ(0, empty_bitset.Count()) << "count on empty object must return zero";
  EXPECT_EQ(16, filled_bitset.Count());
}

TEST_F(DynamicBitsetFixture, ReserveMethodTest) {
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

TEST_F(DynamicBitsetFixture, ShrinkToFitMethodTest) {
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

TEST_F(DynamicBitsetFixture, AllMethodTest) {
  EXPECT_EQ(false, empty_bitset.All()) << "empty object can not containe any bits";
  EXPECT_EQ(true, filled_bitset.All());
}

TEST_F(DynamicBitsetFixture, AnyMethodTest) {
  EXPECT_EQ(false, empty_bitset.Any()) << "empty object can not contain any bits";
  EXPECT_EQ(true, filled_bitset.Any());
}

TEST_F(DynamicBitsetFixture, NoneMethodTest) {
  EXPECT_EQ(true, empty_bitset.None()) << "empty object can not contain any bits";
  EXPECT_EQ(false, filled_bitset.None());
}

TEST_F(DynamicBitsetFixture, EmptyMethodTest) {
  EXPECT_EQ(true, empty_bitset.Empty()) << "empty object can not contain any bits";
  EXPECT_EQ(false, filled_bitset.Empty());
}

TEST_F(DynamicBitsetFixture, ClearMethodTest) {
  empty_bitset.Clear();
  filled_bitset.Clear();

  EXPECT_EQ(empty_bitset.Size(), filled_bitset.Size()) << "Every object must be empty after clear";
  EXPECT_EQ(empty_bitset.Capacity(), filled_bitset.Capacity()) << "Every object must be empty after clear";
  EXPECT_EQ(empty_bitset.Data(), filled_bitset.Data()) << "Every object must be empty after clear";
}

TEST_F(DynamicBitsetFixture, ResizeMethodTest) {
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

TEST_F(DynamicBitsetFixture, PushBackMethodTest) {
  for (auto i = std::size_t{}; i < 10; ++i) {
    empty_bitset.PushBack(true);
    filled_bitset.PushBack(true);
  }

  EXPECT_EQ(10, empty_bitset.Count());
  EXPECT_EQ(26, filled_bitset.Count());
}

TEST_F(DynamicBitsetFixture, PopBackMethodTest) {
  for (auto i = std::size_t{}; i < 16; ++i) {
    filled_bitset.PopBack();
  }
  EXPECT_EQ(0, filled_bitset.Size());
}

TEST_F(DynamicBitsetFixture, SetIndexMethodTest) {
  EXPECT_THROW(empty_bitset.Set(10), std::out_of_range);

  for (auto i = std::size_t{}; i < 4; ++i) {
    filled_bitset.Set(i, false);
  }

  EXPECT_EQ("0000111111111111", filled_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, SetMethodTest) {
  EXPECT_THROW(empty_bitset.Set(), std::out_of_range);

  auto test_vector = bits::DynamicBitset{16};
  test_vector.Set();

  EXPECT_EQ(false, test_vector.None()) << "'Set' method must set all bits to true";
}

TEST_F(DynamicBitsetFixture, ResetIndexMethodTest) {
  EXPECT_THROW(empty_bitset.Reset(10), std::out_of_range);

  for (auto i = std::size_t{}; i < 4; ++i) {
    filled_bitset.Reset(i);
  }

  EXPECT_EQ("0000111111111111", filled_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, ResetMethodTest) {
  EXPECT_THROW(empty_bitset.Reset(), std::out_of_range);

  filled_bitset.Reset();

  EXPECT_EQ(true, filled_bitset.None()) << "Reset method must set all bits to false";
}

TEST_F(DynamicBitsetFixture, FlipIndexMethodTest) {
  EXPECT_THROW(empty_bitset.Flip(10), std::out_of_range);

  for (auto i = std::size_t{}; i < 4; ++i) {
    filled_bitset.Flip(i);
  }

  EXPECT_EQ("0000111111111111", filled_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, FlipMethodTest) {
  EXPECT_THROW(empty_bitset.Flip(), std::out_of_range);

  filled_bitset.Flip();

  EXPECT_EQ(true, filled_bitset.None());
}

TEST_F(DynamicBitsetFixture, SwapMethodTest) {
  empty_bitset.Swap(filled_bitset);

  EXPECT_EQ(16, empty_bitset.Size());
  EXPECT_EQ(64, empty_bitset.Capacity());
  EXPECT_EQ(16, empty_bitset.Count());
  EXPECT_EQ(0, filled_bitset.Size());
  EXPECT_EQ(0, filled_bitset.Capacity());
  EXPECT_EQ(nullptr, filled_bitset.Data());
}

TEST_F(DynamicBitsetFixture, SubscriptOperatorTest) {
  for (auto i = std::size_t{}; i < 10; ++i) {
    ASSERT_EQ(true, static_cast<bool>(filled_bitset[i]));
    filled_bitset[i] = false;
    ASSERT_EQ(false, static_cast<bool>(filled_bitset[i]));
    filled_bitset[i] = true;
    ASSERT_EQ(true, static_cast<bool>(filled_bitset[i]));
  }
}

TEST_F(DynamicBitsetFixture, AtMethodTest) {
  EXPECT_THROW((void) empty_bitset.At(10), std::out_of_range);
  EXPECT_THROW((void) filled_bitset.At(20), std::out_of_range);

  EXPECT_EQ(true, static_cast<bool>(filled_bitset.At(0)));
  filled_bitset.Reset(0);
  EXPECT_EQ(false, static_cast<bool>(filled_bitset.At(0)));
}

TEST_F(DynamicBitsetFixture, FrontMethodTest) {
  EXPECT_EQ(true, static_cast<bool>(filled_bitset.Front()));
  filled_bitset.Reset(0);
  EXPECT_EQ(false, static_cast<bool>(filled_bitset.Front()));
}

TEST_F(DynamicBitsetFixture, BackMethodTest) {
  EXPECT_EQ(true, static_cast<bool>(filled_bitset.Back()));
  filled_bitset.Reset(15);
  EXPECT_EQ(false, static_cast<bool>(filled_bitset.Back()));
}

TEST_F(DynamicBitsetFixture, CopyAssignmentOperatorTest) {
  empty_bitset = filled_bitset;

  EXPECT_EQ(empty_bitset.Size(), filled_bitset.Size());
  EXPECT_EQ(empty_bitset.Capacity(), filled_bitset.Capacity());
  ASSERT_NE(nullptr, empty_bitset.Data());
  EXPECT_EQ(empty_bitset.ToString(), filled_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, MoveAssignmentOperatorTest) {
  empty_bitset = std::move(filled_bitset);

  EXPECT_EQ(16, empty_bitset.Size());
  EXPECT_EQ(64, empty_bitset.Capacity());
  EXPECT_EQ(16, empty_bitset.Count());
  EXPECT_EQ(0, filled_bitset.Size());
  EXPECT_EQ(0, filled_bitset.Capacity());
  EXPECT_EQ(nullptr, filled_bitset.Data());
}

TEST_F(DynamicBitsetFixture, BitwiseAndAssignmentOperatorTest) {
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

TEST_F(DynamicBitsetFixture, BitwiseOrAssignmentOperatorTest) {
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

TEST_F(DynamicBitsetFixture, BitwiseXorAssignmentOperatorTest) {
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

TEST_F(DynamicBitsetFixture, BitwiseInverseOperatorTest) {
  EXPECT_THROW((void) ~empty_bitset, std::out_of_range);

  empty_bitset = ~filled_bitset;

  ASSERT_EQ(16, empty_bitset.Size());
  ASSERT_EQ(64, empty_bitset.Capacity());
  ASSERT_EQ(true, empty_bitset.None());
}

TEST_F(DynamicBitsetFixture, ToStringMethodTest) {
  EXPECT_EQ("", empty_bitset.ToString());
  EXPECT_EQ("1111111111111111", filled_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, LhsAssignmentTest) {
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

TEST_F(DynamicBitsetFixture, RhsAssignmentTest) {
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

TEST_F(DynamicBitsetFixture, CopyLhsTest) {
  empty_bitset = filled_bitset << 8;

  ASSERT_EQ("1111111100000000", empty_bitset.ToString());

  empty_bitset = filled_bitset << 1;

  EXPECT_EQ("1111111111111110", empty_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, CopyRhsTest) {
  empty_bitset = filled_bitset >> 8;

  ASSERT_EQ("0000000011111111", empty_bitset.ToString());

  empty_bitset = filled_bitset >> 1;

  EXPECT_EQ("0111111111111111", empty_bitset.ToString());
}

TEST_F(DynamicBitsetFixture, AdvancedLhsRhsTest) {
  for (auto i = std::size_t{}; i < 16; ++i) {
    filled_bitset >>= 1;
    ASSERT_EQ(false, static_cast<bool>(filled_bitset[i]));
  }

  filled_bitset.Set();

  for (auto i = std::size_t{}; i < 16; ++i) {
    filled_bitset <<= 1;
    ASSERT_EQ(false, static_cast<bool>(filled_bitset[15 - i]));
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

TEST_F(DynamicBitsetFixture, IteratorPredicateTest) {
  auto begin_iterator = filled_bitset.begin();
  auto end_iterator = filled_bitset.end();

  EXPECT_EQ(false, begin_iterator == end_iterator);
  EXPECT_EQ(true, begin_iterator != end_iterator);
  EXPECT_EQ(true, begin_iterator <= end_iterator);
  EXPECT_EQ(true, begin_iterator < end_iterator);
  EXPECT_EQ(false, begin_iterator >= end_iterator);
  EXPECT_EQ(false, begin_iterator > end_iterator);
}

TEST_F(DynamicBitsetFixture, IteratorArithmeticTest) {
  auto begin_iterator = filled_bitset.begin();
  auto end_iterator = filled_bitset.end();
  auto bitset_size = filled_bitset.Size();

  EXPECT_EQ(bitset_size, end_iterator - begin_iterator);
  ASSERT_EQ(false, begin_iterator + 0 == end_iterator);
  EXPECT_EQ(true, begin_iterator + bitset_size == end_iterator);
  ASSERT_EQ(true, begin_iterator + 0 != end_iterator);
  EXPECT_EQ(false, begin_iterator + bitset_size != end_iterator);
}

namespace test {

constexpr auto kIsTruePredicate = [] [[nodiscard]] (auto&& value) constexpr noexcept -> bool {
  return static_cast<bool>(std::forward<decltype(value)>(value));
};

constexpr auto kIsFalsePredicate = [] [[nodiscard]] (auto&& value) constexpr noexcept -> bool {
  return !static_cast<bool>(std::forward<decltype(value)>(value));
};

}  // namespace test

TEST_F(DynamicBitsetFixture, IteratorBitwiseTraverseTest) {
  EXPECT_EQ(true, std::all_of(filled_bitset.begin(), filled_bitset.end(), test::kIsTruePredicate));
  EXPECT_EQ(true, std::none_of(filled_bitset.begin(), filled_bitset.end(), test::kIsFalsePredicate));
}

TEST_F(DynamicBitsetFixture, IteratorBitwiseOrTest) {
  {
    auto bitset = filled_bitset;
    std::for_each(bitset.begin(), bitset.end(), [](auto&& value) constexpr noexcept -> void {
      std::forward<decltype(value)>(value) |= false;
    });
    EXPECT_EQ(true, std::all_of(bitset.cbegin(), bitset.cend(), test::kIsTruePredicate));
    std::for_each(bitset.begin(), bitset.end(), [](auto&& value) constexpr noexcept -> void {
      std::forward<decltype(value)>(value) |= false;
    });
    EXPECT_EQ(true, std::all_of(bitset.cbegin(), bitset.cend(), test::kIsTruePredicate));
  }
}

TEST_F(DynamicBitsetFixture, IteratorBitwiseAndTest) {
  std::for_each(filled_bitset.begin(), filled_bitset.end(), [](auto&& value) constexpr noexcept -> void {
    std::forward<decltype(value)>(value) &= true;
  });
  EXPECT_EQ(true, std::all_of(filled_bitset.cbegin(), filled_bitset.cend(), test::kIsTruePredicate));
  std::for_each(filled_bitset.begin(), filled_bitset.end(), [](auto&& value) constexpr noexcept -> void {
    std::forward<decltype(value)>(value) &= false;
  });
  EXPECT_EQ(true, std::none_of(filled_bitset.cbegin(), filled_bitset.cend(), test::kIsTruePredicate));
}

TEST_F(DynamicBitsetFixture, IteratorBitwiseXorTest) {
  std::for_each(filled_bitset.begin(), filled_bitset.end(), [](auto&& value) constexpr noexcept -> void {
    std::forward<decltype(value)>(value) ^= false;
  });
  EXPECT_EQ(true, std::all_of(filled_bitset.cbegin(), filled_bitset.cend(), test::kIsTruePredicate));
  std::for_each(filled_bitset.begin(), filled_bitset.end(), [](auto&& value) constexpr noexcept -> void {
    std::forward<decltype(value)>(value) ^= true;
  });
  EXPECT_EQ(true, std::none_of(filled_bitset.cbegin(), filled_bitset.cend(), test::kIsTruePredicate));
}

TEST_F(DynamicBitsetFixture, StressTest) {
  auto test_vector = bits::DynamicBitset{};

  constexpr auto kMaxTestSize = std::size_t{std::numeric_limits<int>::max()};

  for (auto i = std::size_t{}; i < kMaxTestSize; ++i) {
    test_vector.PushBack(true);
  }

  ASSERT_EQ(kMaxTestSize, test_vector.Count());

  for (auto i = std::size_t{}; i < kMaxTestSize; ++i) {
    test_vector.PopBack();
  }

  ASSERT_EQ(0, test_vector.Size());

  test_vector.ShrinkToFit();

  constexpr auto kMidTestSize = std::size_t{kMaxTestSize >> 1};

  test_vector.Resize(kMaxTestSize, true);

  test_vector >>= kMidTestSize;

  ASSERT_EQ(kMidTestSize + 1, test_vector.Count());

  test_vector <<= kMidTestSize;

  ASSERT_EQ(kMidTestSize + 1, test_vector.Count());

  test_vector.Clear();
}

TEST_F(DynamicBitsetFixture, TemplateAllocatorTest) {
  constexpr auto kBufferSize = std::size_t{1000};
  auto buffer = std::array<std::size_t, kBufferSize>{};
  auto buffer_resource = std::pmr::monotonic_buffer_resource{buffer.data(), kBufferSize};
  auto allocator = std::pmr::polymorphic_allocator<std::size_t>{&buffer_resource};
  auto test_vector = bits::DynamicBitset<std::size_t, decltype(allocator)>{allocator};

  constexpr auto kMaxTestSize = std::size_t{7200};  // NOLINT

  for (auto i = std::size_t{}; i < kMaxTestSize; ++i) {
    test_vector.PushBack(true);
  }

  ASSERT_EQ(kMaxTestSize, test_vector.Count());

  for (auto i = std::size_t{}; i < kMaxTestSize; ++i) {
    test_vector.PopBack();
  }

  ASSERT_EQ(true, test_vector.None());

  test_vector.ShrinkToFit();

  constexpr auto kMidTestSize = std::size_t{kMaxTestSize >> 1};

  test_vector.Resize(kMaxTestSize, true);

  test_vector >>= kMidTestSize;

  ASSERT_EQ(kMidTestSize, test_vector.Count());

  test_vector <<= kMidTestSize;

  ASSERT_EQ(kMidTestSize, test_vector.Count());

  test_vector.Clear();
}
