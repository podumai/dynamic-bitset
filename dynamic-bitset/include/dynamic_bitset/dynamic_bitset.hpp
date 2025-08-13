/**
 * @file dynamic_bitset/dynamic_bitset.hpp
 * @date 25-07-2025
 * @version 2.0.0
 * @copyright MIT License
 *
 * @author Kirill Morozov kirillsm05@gmail.com
 */

/**
 * @brief Main `DynamicBitset` container and operations
 * @defgroup dynamic-bitset-main Core API
 */

/**
 * @brief Alias definitions in `DynamicBitset` class
 * @defgroup dynamic-bitset-aliases
 * @ingroup dynamic-bitset-main
 */

/**
 * @brief Iterator types and range operations
 * @defgroup dynamic-bitset-iterators
 * @ingroup dynamic-bitset-main
 */

/**
 * @brief Element access operations
 * @defgroup dynamic-bitset-access
 * @ingroup dynamic-bitset-main
 */

/**
 * @brief Operations modifying the object state (general)
 * @defgroup dynamic-bitset-modifiers
 * @ingroup dynamic-bitset-main
 */

/**
 * @brief Capacity operations
 * @defgroup dynamic-bitset-Capacity
 * @ingroup dynamic-bitset-main
 */

/**
 * @brief Bit-level manipulation (Set/Reset/Test/Flip)
 * @defgroup dynamic-bitset-bitops Bit operations
 * @ingroup dynamic-bitset-main
 */

/**
 * @brief `std::format` integration and string conversion
 * @defgroup dynamic-bitset-format Formatting
 * @ingroup dynamic-bitset-main
 */

#pragma once

#if defined(func)
  #pragma push_macro("func")
  #define BITS_FUNC_MACRO_REDEFINED
  #undef func
  #define func auto
#else
  #define func auto
#endif

#if !defined(NDEBUG)
  #include <cassert>
  #define BITS_DYNAMIC_BITSET_ASSERT(...) assert(__VA_ARGS__)
#else
  #define BITS_DYNAMIC_BITSET_ASSERT(...)
#endif

#include <bit>
#include <climits>   /* CHAR_BIT */
#include <concepts>  /* std::unsigned_integral */
#include <cstdint>   /* std::size_t, std::ptrdiff_t */
#include <format>    /* std::format */
#include <iterator>  /* iterator_traits, Iterator concepts */
#include <memory>    /* std::allocator<T> */
#include <stdexcept> /* std::out_of_range, std::length_error, std::invalid_argument */

#if CHAR_BIT != 8
  #error "bits::DynamicBitset only works on platforms with 8 bits per byte."
#endif

namespace __bits_details
{

/**
 * @brief Validates the passed Block type.
 * @details Validates the passed Block type to be unsigned integral type
 *          except `bool`, checks type if is power of two.
 * @concept IsValidDynamicBitsetBlockType
 */
template<typename Block>
concept IsValidDynamicBitsetBlockType =
  std::unsigned_integral<Block> && !std::is_same_v<Block, bool> && !(sizeof(Block) & (sizeof(Block) - 1));

/**
 * @brief Validates the passed Allocator type.
 * @details Validates the passed Allocator type that uses unsigned integral type
 *          except `bool`, checks `value_type` fieled to be power of two.
 * @concept IsValidDynamicBitsetAllocType
 */
template<typename Alloc>
concept IsValidDynamicBitsetAllocType = std::unsigned_integral<typename std::allocator_traits<Alloc>::value_type> &&
                                        !std::is_same_v<typename std::allocator_traits<Alloc>::value_type, bool> &&
                                        !(sizeof(typename std::allocator_traits<Alloc>::value_type) &
                                          (sizeof(typename std::allocator_traits<Alloc>::value_type) - 1));

/**
 * @brief Validates the passed Block iterator type.
 * @details Validates the passed Block iterator type to be convertible to block type.
 * @concept IsValidDynamicBitsetBlockIterator
 */
template<typename BlockIterator, typename TargetBlock>
concept IsValidDynamicBitsetBlockIterator = std::is_convertible_v<decltype(*BlockIterator{}), TargetBlock>;

}  // namespace __bits_details

/**
 * @brief Namespace containing `DynamicBitset` implementation.
 * @namespace bits
 */
namespace bits
{

template<__bits_details::IsValidDynamicBitsetBlockType Block, __bits_details::IsValidDynamicBitsetAllocType Alloc>
class DynamicBitset;

template<typename Alloc = std::allocator<size_t>>
DynamicBitset()
  /* clang-format off */ -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>; /* clang-format on */

template<typename Alloc = std::allocator<size_t>>
DynamicBitset(typename std::allocator_traits<Alloc>::size_type)
  /* clang-format off */ -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>; /* clang-format on */

template<typename Alloc = std::allocator<size_t>>
DynamicBitset(
  typename std::allocator_traits<Alloc>::size_type, typename std::allocator_traits<Alloc>::value_type
) /* clang-format off */ -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>; /* clang-format on */

template<typename Alloc = std::allocator<size_t>>
DynamicBitset(
  typename std::allocator_traits<Alloc>::size_type, typename std::allocator_traits<Alloc>::value_type, const Alloc&
) /* clang-format off */ -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>; /* clang-format on */

/**
 * @brief Container representing a set of bits.
 *
 * @ingroup dynamic-bitset-main
 *
 * @class DynamicBitset
 * @tparam Block Unsigned integral type used for bit storage.
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 *
 * @details `DynamicBitset` provides an abstraction over a bit sequence.
 *          This container support vector-like interface with bitwise operations support.
 */
template<
  __bits_details::IsValidDynamicBitsetBlockType Block = size_t,
  __bits_details::IsValidDynamicBitsetAllocType Alloc = std::allocator<Block>>
class DynamicBitset
{
 public:
  class Iterator;

 public:
  /**
   * @public
   * @ingroup dynamic-bitset-aliases
   * @{
   */
  /**
   * @brief An alias representing block type using snake case.
   * @typedef block_type
   */
  using block_type = Block;
  /**
   * @brief An alias representing block type using camel case.
   * @typedef blockType
   */
  using blockType = Block;
  /**
   * @brief An alias representing allocator type using snake case.
   * @typedef allocator_type
   */
  using allocator_type = Alloc;
  /**
   * @brief An alias representing allocator type using camel case.
   * @typedef allocatorType
   */
  using allocatorType = Alloc;
  /**
   * @brief An alias representing size type using snake case.
   * @typedef size_type
   */
  using size_type = typename std::allocator_traits<allocator_type>::size_type;
  /**
   * @brief An alias representing size type using camel case.
   * @typedef sizeType
   */
  using sizeType = typename std::allocator_traits<allocatorType>::size_type;
  /**
   * @brief An alias representing pointer difference type using snake case.
   * @typedef difference_type
   */
  using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
  /**
   * @brief An alias representing pointer difference type using camel case.
   * @typedef differenceType
   */
  using differenceType = typename std::allocator_traits<allocatorType>::difference_type;
  /**
   * @brief An alias representing pointer type.
   * @typedef pointer
   */
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  /**
   * @brief An alias representing pointer to contant type using snake case.
   * @typedef const_pointer
   */
  using const_pointer = const typename std::allocator_traits<allocator_type>::pointer;
  /**
   * @brief An alias representing pointer to constant type using camel case.
   * @typedef constPointer
   */
  using constPointer = const typename std::allocator_traits<allocatorType>::pointer;
  /**
   * @brief An alias representing logical value type using snake case.
   * @typedef value_type
   *
   * @note This alias type do not represent the internal value type.
   */
  using value_type = bool;
  /**
   * @brief An alias representing logical value type using camel case.
   * @typedef valueType
   *
   * @note This alias type do not represent the internal value type.
   */
  using valueType = bool;
  /**
   * @brief An alias representing iterator type.
   * @typedef iterator
   */
  using iterator = Iterator;
  /**
   * @brief An alias representing constant iterator type using snake case.
   * @typedef const_iterator
   */
  using const_iterator = const Iterator;
  /**
   * @brief An alias representing constant iterator type using camel case.
   * @typedef ConstIterator
   */
  using ConstIterator = const Iterator;
  /// @}

 private:
  enum BitMask : blockType
  {
    kUnknown,
    kBit = 1,
    kReset = 0,
    kSet = static_cast<blockType>(-1)
  };

  struct BlockInfo final
  {
    static constexpr sizeType bitsCount{std::numeric_limits<blockType>::digits};
    static constexpr sizeType byteDivConst{bitsCount < 32 ? sizeof(blockType) + 2 : bitsCount == 32 ? 5 : 6};
    static constexpr sizeType byteModConst{bitsCount - 1};
  };

 public:
  /**
   * @public
   * @class Iterator
   * @ingroup dynamic-bitset-iterators
   * @brief Random-access iterator for `DynamicBitset`.
   *
   * @warning Invalidated when `DynamicBitset` reallocation takes place.
   */
  class Iterator
  {
   private:
    friend DynamicBitset;

   public:
    /**
     * @brief An alias representing iterator position difference type using snake case.
     * @typedef difference_type
     */
    using difference_type = typename DynamicBitset::difference_type;
    /**
     * @brief An alias representing iterator position difference type using camel case.
     * @typedef differenceType
     */
    using differenceType = typename DynamicBitset::differenceType;
    /**
     * @brief An alias representing iterator pointer type (not in use).
     * @typedef pointer
     */
    using pointer = typename DynamicBitset::pointer;
    /**
     * @brief An alias representing iterator underlying value type using snake case.
     * @typedef value_type
     */
    using value_type = typename DynamicBitset::value_type;
    /**
     * @brief An alias representing iterator underlying value type using camel case.
     * @typedef value_type
     */
    using valueType = typename DynamicBitset::valueType;
    /**
     * @brief Iterator tag hint for generic algorithms using snake case.
     * @typedef iterator_category
     */
    using iterator_category = typename std::contiguous_iterator_tag;
    /**
     * @brief Iterator tag hint for generic algorithms using camel case.
     * @typedef iteratorCategory
     */
    using iteratorCategory = typename std::contiguous_iterator_tag;

   private:
    /**
     * @class BitWrapper
     * @ingroup dynamic-bitset-iterators
     * @brief Proxy reference to a single bit (since bits arent't addressable).
     * @private
     *
     * @warning Invalidated when `DynamicBitset` reallocation takes place.
     */
    class BitWrapper final
    {
     private:
      friend DynamicBitset;
      friend Iterator;

     public:
      constexpr BitWrapper() noexcept = default;

      constexpr BitWrapper(
        pointer ptr,  //
        differenceType bitPosition
      ) noexcept
        : byte_{ptr}, bit_{bitPosition}
      { }

      constexpr BitWrapper(const BitWrapper& other) noexcept = default;
      constexpr ~BitWrapper() = default;

      constexpr func operator=(
        bool value
      ) noexcept -> BitWrapper&
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        if (value)
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] |=
            DynamicBitset::BitMask::kBit << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
        }
        else
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &=
            ~(DynamicBitset::BitMask::kBit << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst));
        }

        return *this;
      }

     private:
      [[nodiscard]]
      constexpr func GetBit() const noexcept -> bool
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        return byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &
               DynamicBitset::BitMask::kBit << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
      }

      constexpr func SetBit() noexcept -> void
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] |=
          DynamicBitset::BitMask::kBit << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
      }

      constexpr func UnsetBit() noexcept -> void
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &=
          ~(DynamicBitset::BitMask::kBit << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst));
      }

     public:
      constexpr func operator=(
        const BitWrapper& other
      ) noexcept -> BitWrapper&
      {
        BITS_DYNAMIC_BITSET_ASSERT(this != &other);

        if (other.GetBit())
        {
          SetBit();
        }
        else
        {
          UnsetBit();
        }

        return *this;
      }

      constexpr func operator|=(
        bool value
      ) noexcept -> BitWrapper&
      {
        if (value)
        {
          SetBit();
        }
        return *this;
      }

      constexpr func operator&=(
        bool value
      ) noexcept -> BitWrapper&
      {
        if (!value)
        {
          UnsetBit();
        }
        return *this;
      }

      constexpr func operator^=(
        bool value
      ) noexcept -> BitWrapper&
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        if (value)
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] ^=
            DynamicBitset::BitMask::kBit << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
        }
        return *this;
      }

      [[nodiscard]]
      constexpr func operator==(
        const BitWrapper& other
      ) const noexcept -> bool
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ == other.byte_);

        return bit_ == other.bit_;
      }

      [[nodiscard]]
      constexpr func operator!=(
        const BitWrapper& other
      ) const noexcept -> bool
      {
        return !(*this == other);
      }

      constexpr operator bool() const noexcept
      {
        return byte_ != nullptr ? GetBit() : false;
      }

      explicit constexpr operator char() const noexcept
      {
        return byte_ != nullptr ? GetBit() | '0' : '0';
      }

      explicit constexpr operator DynamicBitset::blockType() const noexcept
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        return byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst];
      }

     private:
      pointer byte_;
      differenceType bit_;
    };

   public:
    /**
     * @public
     * @brief Default `Iterator` constructor (defaulted).
     *
     * @throws None (no-throw guarantee).
     */
    constexpr Iterator() noexcept = default;

   private:
    constexpr Iterator(
      pointer ptr,  //
      differenceType bitPosition
    ) noexcept
      : wrapper_{ptr, bitPosition}
    { }

   public:
    /**
     * @public
     * @brief Copy constructor for `Iterator` object (defaulted).
     *
     * @throws None (no-throw guarantee).
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset bits{10};
     * auto iter1{bits.begin()}; // iter1 points to the first bit
     * decltype(iter1) iter2{iter1}; // iter2 points to the first bit as iter1
     * @endcode
     */
    constexpr Iterator(const Iterator&) noexcept = default;

    /**
     * @public
     * @brief Destructor for `Iterator` object (defaulted).
     *
     * @throws None (no-throw guarantee).
     */
    constexpr ~Iterator() = default;

    /**
     * @public
     * @brief Exchanges the contents of this iterator with another.
     *
     * @param[in] other Iterator to swap with.
     *
     * @throws None (no-throw guarantee).
     *
     * @note Invalidates no iterators.
     *
     * @par Example:
     * @code{.cpp}
     * auto it1{bits.begin()};
     * auto it2{bits.end() - 1};
     * it1.Swap(it2);
     * @endcode
     */
    constexpr func Swap(
      Iterator& other
    ) noexcept -> void
    {
      BITS_DYNAMIC_BITSET_ASSERT(this != &other);

      wrapper_.Swap(other.wrapper_);
    }

    /**
     * @public
     * @brief Unary prefix increment operator overload for `Iterator` object
     * @details Advances `Iterator` to point to the next bit in sequence.
     *
     * @throws None (no-throw guarantee).
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset bits{10};
     * for (auto iter{bits.begin()}; iter != bits.end(); ++iter)
     * {
     *   // ...
     * }
     * @endcode
     */
    constexpr func operator++() noexcept -> Iterator&
    {
      ++wrapper_.bit_;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee)
     */
    constexpr func operator++() const noexcept -> const Iterator&
    {
      ++wrapper_.bit_;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator++(
      int
    ) noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_++};
    }

    /**
     * @public
     * @throws None (no-throw guarantee)
     */
    constexpr func operator++(
      int
    ) const noexcept -> const Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_++};
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee).
     */
    constexpr func operator+=(
      differenceType index
    ) noexcept -> Iterator&
    {
      wrapper_.bit_ += index;
      return *this;
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee)
     */
    constexpr func operator+=(
      differenceType index
    ) const noexcept -> const Iterator&
    {
      wrapper_.bit_ += index;
      return *this;
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]]
    constexpr func operator+(
      differenceType index
    ) const noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_ + index};
    }

    /**
     * @param[in] index
     * @param[in] iter
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]]
    friend constexpr func operator+(
      differenceType index,  //
      const Iterator& iter
    ) noexcept -> Iterator
    {
      return {iter.wrapper_.byte_, iter.wrapper_.bit_ + index};
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator--() noexcept -> Iterator&
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ > 0);

      --wrapper_.bit_;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee)
     */
    constexpr func operator--() const noexcept -> const Iterator&
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit >> 0);

      --wrapper_.bit_;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator--(
      int
    ) noexcept -> Iterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ > 0);

      return {wrapper_.byte_, wrapper_.bit_--};
    }

    /**
     * @public
     * @throws None (no-throw guarantee)
     */
    constexpr func operator--(
      int
    ) const noexcept -> const Iterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ > 0);

      return {wrapper_.byte_, wrapper_.bit_--};
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee).
     */
    constexpr func operator-=(
      differenceType index
    ) noexcept -> Iterator&
    {
      wrapper_.bit_ -= index;

      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ >= 0);

      return *this;
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee)
     */
    constexpr func operator-=(
      differenceType index
    ) const noexcept -> const Iterator&
    {
      wrapper_.bit_ -= index;

      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ >= 0);

      return *this;
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]]
    constexpr func operator-(
      differenceType index
    ) const noexcept -> Iterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ - index >= 0);

      return {wrapper_.byte_, wrapper_.bit_ - index};
    }

    /**
     * @public
     * @param[in] other
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]]
    constexpr func operator-(
      const Iterator& other
    ) const noexcept -> differenceType
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.byte_ == other.wrapper_.byte_);

      return wrapper_.bit_ - other.wrapper_.bit_;
    }

    /**
     * @brief Compare `lhs` and `rhs` bit positions.
     *
     * @param[in] lhs First `Iterator` operand.
     * @param[in] rhs Second `Iterator` operand.
     * @return `true` if `lhs` points to bit position before `rhs` bit position,
     * `false` otherwise.
     *
     * @throws None (no-throw guarantee).
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset bits{10};
     * assert(bits.begin() < bits.end());
     * @endcode
     */
    [[nodiscard]]
    friend constexpr func operator<(
      const Iterator& lhs,  //
      const Iterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ < rhs.wrapper_.bit_;
    }

    /**
     * @brief Compare `lhs` and `rhs` bit positions.
     *
     * @param[in] lhs First `Iterator` operand.
     * @param[in] rhs Second `Iterator` operand.
     * @return `true` if `lhs` points to bit position before or equals to `rhs`
     * bit position, `false` otherwise.
     *
     * @throws None (no-throw guarantee).
     *
     * @warning Comparing iterators from different `DynamicBitset` objects lead
     * to undefined behaviour.
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset bits{10};
     * assert(bits.begin() <= bits.end());
     * @endcode
     */
    [[nodiscard]]
    friend constexpr func operator<=(
      const Iterator& lhs,  //
      const Iterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ <= rhs.wrapper_.bit_;
    }

    /**
     * @brief Compare `lhs` and `rhs` bit positions.
     *
     * @param[in] lhs First `Iterator` operand.
     * @param[in] rhs Second `Iterator` operand.
     * @return `true` if `lhs` points to bit position beyound `rhs` bit
     * position, `false` otherwise.
     *
     * @throws None (no-throw guarantee).
     *
     * @warning Comparing iterators from different `DynamicBitset` objects lead
     * to undefined behaviour.
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset bits{10};
     * assert(bits.begin() + 5 > bits.begin());
     * @endcode
     */
    [[nodiscard]]
    friend constexpr func operator>(
      const Iterator& lhs,  //
      const Iterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ > rhs.wrapper_.bit_;
    }

    /**
     * @brief Compare `lhs` and `rhs` bit positions.
     *
     * @param[in] lhs First `Iterator` operand.
     * @param[in] rhs Second `Iterator` operand.
     * @return `true` if `lhs` points to bit position beyound or equals `rhs`
     * bit position, `false` otherwise.
     *
     * @throws None (no-throw guarantee).
     *
     * @warning Comparing iterators from different `DynamicBitset` objects lead
     * to undefined behaviour.
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset bits{10};
     * assert(bits.begin() + 5 >= bits.begin());
     * @endcode
     */
    [[nodiscard]]
    friend constexpr func operator>=(
      const Iterator& lhs,  //
      const Iterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ >= rhs.wrapper_.bit_;
    }

    /**
     * @brief Compare `lhs` and `rhs` bit positions.
     *
     * @param[in] lhs First `Iterator` operand.
     * @param[in] rhs Second `Iterator` operand.
     * @return `true` if bit positions equal, `false` otherwise.
     *
     * @throws None (no-throw guarantee).
     *
     * @warning Comparing iterators from different `DynamicBitset` objects lead
     * to undefined behaviour.
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset a{10};
     * for (auto iter{a.begin()}; !(iter == a.end()); ++iter)
     * {
     *   // ...
     * }
     * @endcode
     */
    [[nodiscard]]
    friend constexpr func operator==(
      const Iterator& lhs,  //
      const Iterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ == rhs.wrapper_.bit_;
    }

    /**
     * @brief Compare `lhs` and `rhs` bit positions
     *
     * @param[in] lhs First `Iterator` operand.
     * @param[in] rhs Second `Iterator` operand.
     * @return `true` if bit positions unequal, `false` otherwise.
     *
     * @throws None (no-throw guarantee).
     *
     * @warning Comparing iterators from different `DynamicBitset` objects lead
     * to undefined behaviour.
     *
     * @par Example:
     * @code{.cpp}
     * bits::DynamicBitset a{10};
     * for (auto iter{a.begin()}; iter != a.end(); ++iter)
     * {
     *   // ...
     * }
     * @endcode
     */
    [[nodiscard]]
    friend constexpr func operator!=(
      const Iterator& lhs,  //
      const Iterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_byte_);

      return lhs.wrapper_.bit_ != rhs.wrapper_.bit_;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]]
    constexpr func operator*() noexcept -> BitWrapper&
    {
      return wrapper_;
    }

    /**
     * @public
     * @throws None (no-throw guarantee)
     */
    [[nodiscard]]
    constexpr func operator*() const noexcept -> bool
    {
      return wrapper_.GetBit();
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]]
    constexpr func operator[](
      differenceType index
    ) noexcept -> BitWrapper
    {
      return {wrapper_.byte_, wrapper_.bit_ + index};
    }

    /**
     * @public
     * @param[in] index
     * @throws None (no-throw guarantee)
     */
    [[nodiscard]]
    constexpr func operator[](
      differenceType index
    ) const noexcept -> bool
    {
      return *(*this + index);
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr Iterator& operator=(const Iterator&) noexcept = default;

   private:
    mutable BitWrapper wrapper_;
  };

 private:
  [[nodiscard]]
  static constexpr func CalculateCapacity(
    sizeType bits
  ) noexcept -> sizeType
  {
    return (bits >> BlockInfo::byteDivConst) + (bits & BlockInfo::byteModConst ? 1 : 0);
  }

  constexpr func SetBit(
    sizeType index,  //
    bool value
  ) noexcept -> void
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    if (value)
    {
      storage_[index >> BlockInfo::byteDivConst] |= BitMask::kBit << (index & BlockInfo::byteModConst);
    }
    else
    {
      storage_[index >> BlockInfo::byteDivConst] &= ~(BitMask::kBit << (index & BlockInfo::byteModConst));
    }
  }

  [[nodiscard]]
  constexpr func ResizeFactor() const noexcept -> bool
  {
    return (bits_ >> BlockInfo::byteDivConst) >= blocks_;
  }

  static constexpr func CopyData(
    pointer source,       //
    pointer destination,  //
    sizeType n
  ) noexcept -> void
  {
    BITS_DYNAMIC_BITSET_ASSERT(source != nullptr && destination != nullptr);

    pointer end{source + n};

    [[likely]]
    while (source < end)
    {
      *destination++ = *source++;
    }
  }

  static constexpr func FillData(
    pointer source,  //
    sizeType n,      //
    sizeType value
  ) noexcept -> void
  {
    BITS_DYNAMIC_BITSET_ASSERT(source != nullptr);

    pointer end{source + n};

    [[likely]]
    while (source < end)
    {
      *source++ = value;
    }
  }

  constexpr func GrowInit() -> void
  {
    const sizeType newSize{blocks_ + (blocks_ >> 1) + 2};
    pointer tempPtr{std::allocator_traits<allocatorType>::allocate(alloc_, newSize, storage_)};

    if (storage_)
    {
      std::copy(storage_, storage_ + blocks_, tempPtr);
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
    }

    std::fill(tempPtr + blocks_, tempPtr + newSize, BitMask::kReset);
    storage_ = tempPtr;
    blocks_ = newSize;
  }

 public:
  /**
   * @public
   * @brief Returns read-write iterator that points to the first bit in
   * `DynamicBitset` object
   * @see Iterator
   * @ingroup dynamic-bitset-iterators
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{10};
   * for (auto iter{bits.begin()}; iter != bits.end(); ++i)
   * {
   *   // ...
   * }
   * @endcode
   */
  [[nodiscard]]
  constexpr func begin() noexcept -> Iterator
  {
    return {storage_, 0};
  }

  /**
   * @public
   * @brief Returns read-write iterator that points beyound the last bit in
   * `DynamicBitset` object
   * @see Iterator
   * @ingroup dynamic-bitset-iterators
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{10};
   * for (auto iter{bits.begin()}; iter != bits.end(); ++iter)
   * {
   *   // ...
   * }
   * @endcode
   */
  [[nodiscard]]
  constexpr func end() noexcept -> Iterator
  {
    return {storage_, static_cast<differenceType>(bits_)};
  }

  /**
   * @public
   * @brief Returns read-only iterator that points to the first bit in
   * `DynamicBitset` object
   * @see Iterator
   * @ingroup dynamic-bitset-iterators
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{10};
   * for (auto iter{bits.cbegin()}; iter != bits.cend(); ++i)
   * {
   *   // ...
   * }
   * @endcode
   */
  [[nodiscard]]
  constexpr func cbegin() const noexcept -> ConstIterator
  {
    return {storage_, 0};
  }

  /**
   * @public
   * @brief Returns read-only iterator that points beyound the last bit in
   * `DynamicBitset` object
   * @see Iterator
   * @ingroup dynamic-bitset-iterators
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{10};
   * for (auto iter{bits.cbegin()}; iter != bits.cend(); ++iter)
   * {
   *   // ...
   * }
   * @endcode
   */
  [[nodiscard]]
  constexpr func cend() const noexcept -> ConstIterator
  {
    return {storage_, static_cast<differenceType>(bits_)};
  }

  /**
   * @public
   * @brief Consturct an empty `DynamicBitset` with default constructed
   * allocator.
   * @details Creates a `DynamicBitset` with zero Capacity and no allocated
   * storage.
   * @ingroup dynamic-bitset-main
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits; // Data() == nullptr, Size() == 0, Capacity() ==
   * 0
   * @endcode
   */
  constexpr DynamicBitset() noexcept(std::is_nothrow_default_constructible_v<allocatorType>) = default;

  /**
   * @public
   * @brief Construct an empty `DynamicBitset` with a specified allocator.
   * @details Creates a `DynamicBitset` with zero Capacity and no allocated
   * storage, but configures it to use the provided `allocator`.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] allocator The allocator to use for subsequent memory
   * allocations.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * CustomAllocatorType customAllocator;
   * bits::DynamicBitset<typename CustomAllocatorType::value_type,
   * CustomAllocatorType> bits{customAllocator}; bits.Resize(1'000, true); //
   * Uses allocator copy of customAllocatorType instance
   * @endcode
   */
  explicit constexpr DynamicBitset(
    const allocatorType& allocator
  ) noexcept
    : alloc_{allocator}
  { }

  /**
   * @public
   * @brief Construct `DynamicBitset` with a specified allocator.
   * @details Creates a `DynamicBitset` with `bits` size, copy-constructed
   * allocator and first block initialized with `value`.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] bits The size of the `DynamicBitset` to construct.
   * @param[in] value The value to initialize the first block.
   * @param[in] allocator The allocator to use for subsequent memory
   * allocations.
   *
   * @throws std::bad_alloc If memory allocation fails.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, 0xff}; // Size() == 100, bits in {0...7} are set
   * to `true`
   *                                   // [1, 1, 1, 1, 1, 1, 1, 1, 0, ...]
   * bits::DynamicBitset b{10, 0, std::allocator<size_t>{}}; // Size() == 10,
   * bits in {0...9} are set to `false`
   *                                                         // Explicitly
   * passed `std::allocator<size_t>` instance
   * @endcode
   */
  constexpr DynamicBitset(
    sizeType bits,        //
    blockType value = 0,  //
    const allocatorType& allocator = allocatorType{}
  )
    : alloc_{allocator}
  {
    if (!bits)
    {
      return;
    }

    sizeType tempSize{CalculateCapacity(bits)};
    storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, tempSize);
    bits_ = bits;
    blocks_ = tempSize;
    std::fill(storage_, storage_ + blocks_, BitMask::kReset);
    *storage_ = value;
  }

  /**
   * @public
   * @brief Copy-constructs a `DynamicBitset` from another instance.
   * @details Constructs a deep copy of `other`, using the copied allocator.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other The `DynamicBitset` to copy resources from.
   *
   * @throws std::bad_alloc If memory allocation fails.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, 0xff};
   * bits::DynamicBitset b{a}; // a == b Two identical `DynamicBitset` instances
   * @endcode
   */
  constexpr DynamicBitset(
    const DynamicBitset& other
  )
    : alloc_{std::allocator_traits<allocatorType>::select_on_container_copy_construction(other.alloc_)}
  {
    if (other.blocks_)
    {
      storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, other.blocks_);
      bits_ = other.bits_;
      blocks_ = other.blocks_;
      std::copy(other.storage_, other.storage_ + other.blocks_, storage_);
    }
  }

  /**
   * @public
   * @brief Copy-constructs a `DynamicBitset` with specified allocator.
   * @details Constructs a deep copy of `other`, using the provided `allocator`
   * for memory allocation.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other The `DynamicBitset` to copy resources from.
   * @param[in] allocator The allocator to use for memory management.
   *
   * @throws std::bad_alloc If memory allocation fails.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, 0xff};
   * bits::DynamicBitset b{a, anotherAllocator}; // a == b except that may holds
   * different allocator instances
   * @endcode
   */
  constexpr DynamicBitset(
    const DynamicBitset& other,  //
    const allocatorType& allocator
  )
    : alloc_{allocator}
  {
    if (other.blocks_)
    {
      storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, other.blocks_);
      bits_ = other.bits_;
      blocks_ = other.blocks_;
      std::copy(other.storage_, other.storage_ + other.blocks_, storage_);
    }
  }

  /**
   * @public
   * @brief Move-constructs a `DynamicBitset` from another instance.
   * @details Transfers ownership of the underlying bit storage from `other`
   *          to the newly constructed object.
   *          After move `other` gets in unspecified state suitable for
   *          destruction or reassignment.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other The `DynamicBitset` to move resources from.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, 0xff};
   * bits::DynamicBitset b{std::move(a)}; // a.Data() == nullptr, a.Size() == 0,
   * a.Capacity() == 0
   * @endcode
   */
  constexpr DynamicBitset(
    DynamicBitset&& other
  ) noexcept
    : storage_{other.storage_}, bits_{other.bits_}, blocks_{other.blocks_}, alloc_{std::move(other.alloc_)}
  {
    other.storage_ = nullptr;
    other.bits_ = other.blocks_ = 0;
  }

  /**
   * @public
   * @brief Move-constructs a `DynamicBitset` with a specified allocator.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other Source `DynamicBitset` to move from.
   * @param[in] allocator Allocator to use for the new `DynamicBitset`.
   *
   * @throws None if `is_always_equal` is true, otherwise may throw
   * `std::bad_alloc`.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, customAllocator};
   * bits::DynamicBitset b{std::move(a), anotherAllocator}; // a is empty if `is_always_equal` is true
   *                                                        // or `customAllocator == anotherAllocator`, unchanged
   *                                                        // otherwise
   * @endcode
   */
  constexpr DynamicBitset(
    DynamicBitset&& other, //
    const allocatorType& allocator
  ) noexcept(std::allocator_traits<allocatorType>::is_always_equal::value)
    : storage_{std::allocator_traits<allocatorType>::is_always_equal::value || allocator == other.alloc_ ? other.storage_ : nullptr}
    , bits_{std::allocator_traits<allocatorType>::is_always_equal::value || allocator == other.alloc_ ? other.bits_ : 0}
    , blocks_{std::allocator_traits<allocatorType>::is_always_equal::value || allocator == other.alloc_ ? other.blocks_ : 0}
    , alloc_{allocator}
  {
    if (std::allocator_traits<allocatorType>::is_always_equal::value || alloc_ == other.alloc_)
    {
      other.storage_ = nullptr;
      other.bits_ = other.blocks_ = 0;
    }
    else
    {
      storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, other.blocks_);
      bits_ = other.bits_;
      blocks_ = other.blocks_;
      std::copy(other.storage_, other.storage_ + other.blocks_, storage_);
    }
  }

  /**
   * @public
   * @ingroup dynamic-bitset-main
   *
   * @tparam BlockIterator.
   *
   * @param[in] first.
   * @param[in] last.
   *
   * @throws std::bad_alloc If memory allocation fails.
   */
  template<__bits_details::IsValidDynamicBitsetBlockIterator<blockType> BlockIterator>
  constexpr DynamicBitset(
    BlockIterator first,  //
    BlockIterator last
  ) noexcept
  {
    sizeType size;
    if constexpr (std::is_same_v<BlockIterator, Iterator> || std::is_same_v<BlockIterator, ConstIterator>)
    {
      size = static_cast<sizeType>(last - first);
    }
    else
    {
      size = static_cast<sizeType>(std::distance(first, last)) << BlockInfo::byteDivConst;
    }
    if (!size)
    {
      return;
    }
    try
    {
      blocks_ = CalculateCapacity(size);
      bits_ = size;
      storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, blocks_);
    }
    catch (const std::exception&)
    {
      blocks_ = bits_ = 0;
      throw;
    }
    pointer end{storage_ + blocks_};
    for (pointer begin{storage_}; begin != end; ++first, ++begin)
    {
      *begin = static_cast<blockType>(*first);
    }
  }

  /**
   * @public
   * @brief Destroys the `DynamicBitset` and releases all allocated memory.
   * @details Safely deallocates the underlying storage using the associated
   * allocator.
   * @ingroup dynamic-bitset-main
   *
   * @throws None (no-throw guarantee).
   */
  constexpr ~DynamicBitset()
  {
    if (storage_)
    {
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
      storage_ = nullptr;
      bits_ = blocks_ = 0;
    }
  }

  /**
   * @public
   * @brief Returns the current storage Capacity in bits.
   * @ingroup dynamic-bitset-Capacity
   *
   * @return The maximum number of bits that can be stored without reallocation.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * bits.Reserve(100); // Capacity() == sizeof(blockType) * 100 * 8, Size() ==
   * 0 bits.PushBack(true); // Capacity() == sizeof(blockType) * 100 * 8,
   * Size() == 1
   * @endcode
   */
  [[nodiscard]]
  constexpr func Capacity() const noexcept -> sizeType
  {
    return blocks_ << BlockInfo::byteDivConst;
  }

  /**
   * @public
   * @brief Returns the theoretical maximum number of bits the container could
   * hold.
   * @details This value reflects fundamental limits of:
   *          - The `sizeType` used (e.g., `size_t` maximum).
   *          - The object representation (no memory constraints considered).
   * @ingroup dynamic-bitset-Capacity
   *
   * @return A constant `sizeType` value equal to
   * `numeric_limits<sizeType>::max()`.
   *
   * @throws None (no-throw guarantee).
   *
   * @warning This is theoretical upper bound. The true usable Capacity may be
   * smaller due to:
   * - Memory availability.
   * - Allocator limitations.
   * - Implementation overhead.
   *
   * @note Identical to `std::vector::max_size()` in behaviour.
   */
  [[nodiscard]]
  constexpr func MaxSize() const noexcept -> sizeType
  {
    return std::numeric_limits<sizeType>::max();
  }

  /**
   * @public
   * @brief Direct read/write access to the underlying bit storage.
   * @ingroup dynamic-bitset-access
   *
   * @return Pointer to the first block of bit storage.
   *
   * @throws None (no-throw guarantee).
   * @warning The returned pointer becomes invalid after:
   *  - Any reallocation (`Resize()`, `Reserve()`, etc).
   *  - Destruction of the `DynamicBitset`.
   *
   * @note Direct comparison with memcmp on pointers may result to non equality
   * due to internal optimizations.
   */
  [[nodiscard]]
  constexpr func Data() noexcept -> pointer
  {
    return storage_;
  }

  /**
   * @public
   * @brief Direct read-only access to the underlying bit storage.
   * @ingroup dynamic-bitset-access
   *
   * @return Const pointer to the first block of bit storage.
   *
   * @throws None (no-throw guarantee).
   * @warning The returned pointer becomes invalid after:
   *  - Any reallocation (`Resize()`, `Reserve()`, etc).
   *  - Destruction of the `DynamicBitset`.
   *
   * @note Direct comparison with memcmp on pointers may result to non equality
   * due to internal optimizations.
   */
  [[nodiscard]]
  constexpr func Data() const noexcept -> constPointer
  {
    return storage_;
  }

  /**
   * @public
   * @brief Returns a copy of the underlying allocator.
   * @ingroup dynamic-bitset-main
   *
   * @return A copy of the allocator used for bit storage management.
   *
   * @throws None (no-throw guarantee).
   */
  [[nodiscard]]
  constexpr func GetAllocator() const noexcept -> allocatorType
  {
    return alloc_;
  }

  /**
   * @public
   * @brief Returns the number of set bits in the `DynamicBitset`.
   * @ingroup dynamic-bitset-main
   *
   * @return The count of set bits (bits with value `true`), uses
   * `std::popcount` and bit extraction.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{8, 0xff};
   * auto setBits{bits.Count()}; // setBits == 8
   * @endcode
   */
  [[nodiscard]]
  constexpr func Count() const noexcept -> sizeType
  {
    if (!bits_)
    {
      return 0;
    }

    pointer end{storage_ + CalculateCapacity(bits_) - 1};
    sizeType bitCount{};

    for (pointer begin{storage_}; begin < end; ++begin)
    {
      bitCount += std::popcount(*begin);
    }

    sizeType remainingBits{bits_ & BlockInfo::byteModConst};
    if (!remainingBits)
    {
      remainingBits = BlockInfo::bitsCount;
    }

    for (sizeType currentBit{}; currentBit < remainingBits; ++currentBit)
    {
      if (*end & BitMask::kBit << currentBit)
      {
        ++bitCount;
      }
    }

    return bitCount;
  }

  /**
   * @public
   * @brief Reduces the Capacity to store the minimum required blocks.
   * @details If bit storage contains unused memory blocks the container resizes
   *          to the minimal Capacity to hold the current size.
   * @ingroup dynamic-bitset-Capacity
   *
   * @throws std::bad_alloc If memory allocation fails (original storage remains
   * intact).
   *
   * @warning If bit storage contains unused blocks:
   *  - Iterators invalidated.
   *  - Pointers to internal storage invalidated.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * bits.Reserve(4); // Size() == 0, Capacity() == 4 * sizeof(blockType) * 8
   * bits.ShrinkToFit(); // Size() == 0, Capacity() == 0
   * bits.Reserve(2); // Size() == 0, Capacity() == 2 * sizeof(blockType) * 8
   * bits.PushBack(true); // Size() == 1, Capacity() == 2 * sizeof(blockType) *
   * 8 bits.ShrinkToFit(); // Size() == 1, Capacity() == sizeof(blockType) * 8
   * @endcode
   */
  constexpr func ShrinkToFit() -> void
  {
    if (!bits_)
    {
      Clear();
      return;
    }

    const sizeType currentBytes{CalculateCapacity(bits_)};

    if (currentBytes < blocks_)
    {
      pointer tempPtr{std::allocator_traits<allocatorType>::allocate(alloc_, currentBytes)};
      std::copy(storage_, storage_ + blocks_, tempPtr);
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
      storage_ = tempPtr;
      blocks_ = currentBytes;
    }
  }

  /**
   * @public
   * @brief Check if any bit is set to `true`.
   * @details Scans the bit storage to find first set bit (bit that is set to
   * `true`).
   * @ingroup dynamic-bitset-main
   *
   * @return `true` if at any bit is set, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * auto isAnySet{bits.Any()}; // Returns `false` because container is empty
   * bits.Resize(100);
   * isAnySet = bits.Any(); // Returns `false` as all bits are unset
   * bits.PushBack(true);
   * isAnySet = bits.Any(); // Returns `true` because the last bit is set
   * @endcode
   */
  [[nodiscard]]
  constexpr func Any() const noexcept -> bool
  {
    if (!bits_)
    {
      return false;
    }

    pointer end{storage_ + CalculateCapacity(bits_) - 1};

    for (pointer begin{storage_}; begin < end; ++begin)
    {
      if (*begin)
      {
        return true;
      }
    }

    sizeType remainingBits{bits_ & BlockInfo::byteModConst};
    if (!remainingBits)
    {
      remainingBits = BlockInfo::bitsCount;
    }

    for (sizeType currentBit{}; currentBit < remainingBits; ++currentBit)
    {
      if (*end & BitMask::kBit << currentBit)
      {
        return true;
      }
    }

    return false;
  }

  /**
   * @public
   * @brief Check if none of the bits are set.
   * @details Scans the bit storage to find at least one set bit (bit that set
   * to `true`). Returns when block with at least one set bit occurres.
   * @ingroup dynamic-bitset-main
   *
   * @return `true` if all bits are unset, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * auto isNoneSet{bits.None()}; // Retruns `true` because container is empty
   * bits.Resize(100);
   * isNoneSet = bits.None(); // Returns `true` because all bits are unset
   * bits.PushBack(true);
   * isNoneSet = bits.None(); // Returns `false` because the last bit is set
   * @endcode
   */
  [[nodiscard]]
  constexpr func None() const noexcept -> bool
  {
    return !Any();
  }

  /**
   * @public
   * @brief Check if all bits are set.
   * @details Scans the bit storage and counts the set bits.
   *          If Size() == Count() returns `true`, `false` otherwise.
   * @see Size(), Count()
   * @ingroup dynamic-bitset-main
   *
   * @return `true` if all bits are set, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * auto isAllSet{bits.All()}; // Returns `false` because container is empty
   * bits.Resize(100, true);
   * isAllSet = bits.All(); // Returns `true` because all bits is set
   * bits.PushBack(false);
   * isAllSet = bits.All(); // Returns `false` because the last bit is unset
   * @endcode
   */
  [[nodiscard]]
  constexpr func All() const noexcept -> bool
  {
    return bits_ && bits_ == Count();
  }

  /**
   * @public
   * @brief Checks if the `DynamicBitset` is empty.
   * @details This method determines whether the underlying bit storage is
   * empty. Equivalent to `Size() == 0`.
   * @see Size()
   * @ingroup dynamic-bitset-Capacity
   *
   * @return `true` If container is empty, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * bool isBitsEmpty{bits.Empty()}; // isBitsEmpty == true
   * bits.Resize(100);
   * isBitsEmpty = bits.Empty(); // isBitsEmpty == false
   * @endcode
   */
  [[nodiscard]]
  constexpr func Empty() const noexcept -> bool
  {
    return !bits_;
  }

  /**
   * @public
   * @brief Releases memory obtained by bit storage.
   * @details Releases memory obtained by bit storage and sets
   *          the container in empty state.
   * @see Empty()
   * @ingroup dynamic-bitset-modifiers
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{100}; // Size() == 100, Capacity() == depeneds on
   * sizeof(size_t)
   *                                // parameter multiplied by CHAR_BIT (size_t
   * is the default block type) bits.Clear(); // Data() == nullptr, Size() == 0,
   * Capacity() == 0
   */
  constexpr func Clear() noexcept -> void
  {
    if (!storage_)
    {
      return;
    }
    std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
    storage_ = nullptr;
    bits_ = blocks_ = 0;
  }

  /**
   * @public
   * @brief Resizes `DynamicBitset` to the given size. If `Size() < bits` then
   * new bits set to `value`.
   * @ingroup dynamic-bitset-modifiers
   *
   * @param[in] bits Bits that `DynamicBitset` will store.
   * @param[in] value Value that will be set to new bits.
   *
   * @throws std::bad_alloc If memory allocation fails.
   *
   * @warning This operation invalidates all iterators if `bits != Size()`.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{5}; // Size() == 5, Sequence: [1, 1, 1, 1, 1]
   * bits.Resize(7); // Size() == 7, Sequence: [1, 1, 1, 1, 1, 0, 0]
   * bits.Resize(100); // Size() == 7, Sequence: [1, 1, 1, 1, 1, 0, 0]
   * bits.Resize(0); // Size() = 0, Sequence: []
   * @endcode
   */
  constexpr func Resize(
    sizeType bits,  //
    bool value = false
  ) -> void
  {
    if (!bits)
    {
      Clear();
      return;
    }
    else if (bits == bits_)
    {
      return;
    }

    const sizeType newSize{CalculateCapacity(bits)};

    if (blocks_ < newSize)
    {
      pointer tempPtr{std::allocator_traits<allocatorType>::allocate(alloc_, newSize)};

      if (storage_)
      {
        std::copy(storage_, storage_ + blocks_, tempPtr);
        std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
      }

      storage_ = tempPtr;

      FillData(storage_ + blocks_, newSize - blocks_, value ? BitMask::kSet : BitMask::kReset);
      blocks_ = newSize;
      bits_ = bits;

      return;
    }

    while (bits_ < bits)
    {
      SetBit(bits_++, value);
    }
  }

  /**
   * @public
   * @brief Reserve amount of blocks to store new bits.
   * @details This operation modifies number of stored blocks, not the stored
   * bits.
   * @ingroup dynamic-bitset-Capacity
   *
   * @param[in] bytes Bytes to append to the underlying storage.
   *
   * @throws std::bad_alloc If memory allocation fails.
   *
   * @warning This operation invalidates all iterators if `blocks > 0`.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits; // Size() == 0, Capacity() == 0
   * bits.Reserve(10); // Size() == 0, Capacity() == sizeof(BlockType) * 10 * 8
   * @endcode
   */
  constexpr func Reserve(
    sizeType blocks
  ) -> void
  {
    if (!blocks)
    {
      return;
    }

    const sizeType newSize{blocks_ + blocks};
    pointer tempPtr{std::allocator_traits<allocatorType>::allocate(alloc_, newSize)};

    if (storage_)
    {
      std::copy(storage_, storage_ + blocks_, tempPtr);
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
    }

    storage_ = tempPtr;
    blocks_ = newSize;
  }

  /**
   * @public
   * @brief Appends a bit to the end of the sequence.
   * @ingroup dynamic-bitset-modifiers
   *
   * @param[in] value The bit value `true/false` to append.
   *
   * @throws std::bad_alloc If memory allocation fails.
   *
   * @warning This operation invalidates all iterators if reallocation occurs,
   *          otherwise `end()` iterator gets changed.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * bits.PushBack(true); // Sequence: [1]
   * bits.PushBack(false); // Sequence: [1, 0]
   * @endcode
   */
  constexpr func PushBack(
    bool value
  ) -> void
  {
    if (ResizeFactor())
    {
      GrowInit();
    }

    SetBit(bits_++, value);
  }

  /**
   * @public
   * @brief Returns the number of bits currently stored in the container.
   * @ingroup dynamic-bitset-Capacity
   *
   * @return The exact count of bits.
   *
   * @throws None (no-throw guarantee).
   * @note Time complexity: O(1).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{128}; // Size() == 128
   * bits.Resize(64); // Size() == 64
   * @endcode
   *
   * @see Capacity()
   * @see Empty()
   */
  [[nodiscard]]
  constexpr func Size() const noexcept -> sizeType
  {
    return bits_;
  }

  /**
   * @public
   * @brief Removes last bit from the sequence.
   * @ingroup dynamic-bitset-modifiers
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined Behaviour** if:
   * - The container is empty `Empty() == true`.
   * @see Size(), Empty()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits(8, 0xff); // Size() == 8, Sequence: [1, 1, 1, 1,
   * 1, 1, 1, 1] bits.PopBack(); // Size() == 7, Sequence: [1, 1, 1, 1, 1, 1,
   * 1]
   * @endcode
   */
  constexpr func PopBack() noexcept -> void
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr && bits_ > 0);

    --bits_;
  }

  /**
   * @public
   * @brief Set the bit with `index` to `value`. Implies range check.
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-based index of the bit to set/unset.
   * @param[in] value The boolean value `true/false`.
   * @return Lvalue reference to `this` object.
   *
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   *
   * @see operator[], Test()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4}; // Sequence: [0, 0, 0, 0]
   * bits.Set(2, true); // Sequence: [0, 0, 1, 0]
   * bits.Set(100); // Throws std::out_of_range exception
   * @endcode
   */
  constexpr func Set(
    sizeType index,  //
    bool value = false
  ) -> DynamicBitset&
  {
    if (index >= bits_)
    {
      throw std::out_of_range{
        std::format("DynamicBitset::Set(sizeType, bool = false): index {} >= Size ({})", index, bits_)
      };
    }

    SetBit(index, value);
    return *this;
  }

  /**
   * @public
   * @brief Set all bits to `true`. Implies range checking.
   * @ingroup dynamic-bitset-main
   *
   * @return Lvalue reference to `this` object.
   *
   * @throws std::out_of_range If `Empty() == true`.
   * @see Empty()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{8}; // Sequence: [0, 0, 0, 0, 0, 0, 0, 0]
   * bits.Set(); // Sequence: [1, 1, 1, 1, 1, 1, 1, 1]
   * bits.Clear(); // Sequence: []
   * bits.Set(); // Throws std::out_of_range exception
   * @endcode
   */
  constexpr func Set() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Set() -> invalid number of bits"};
    }

    FillData(storage_, CalculateCapacity(bits_), BitMask::kSet);
    return *this;
  }

  /**
   * @public
   * @brief Set bit with `index` to `false`. Implies range checking.
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-based index of the bit to reset.
   * @return Lvalue reference to `this` object.
   *
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   * @see Size(), operator[]
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4, 0xff}; // Sequence: [1, 1, 1, 1]
   * bits.Reset(2); // Sequence: [1, 1, 0, 1]
   * bits.Reset(100); // Throws std::out_of_range exception
   * @endcode
   */
  constexpr func Reset(
    sizeType index
  ) -> DynamicBitset&
  {
    if (index >= bits_)
    {
      throw std::out_of_range{std::format("DynamicBitset::Reset(sizeType): index {} >= Size ({})", index, bits_)};
    }

    storage_[index >> BlockInfo::byteDivConst] &= ~(BitMask::kBit << (index & BlockInfo::byteModConst));
    return *this;
  }

  /**
   * @public
   * @brief Set all bits to `false`. Implies range checking.
   * @ingroup dynamic-bitset-main
   *
   * @return Lvalue reference to `this` object.
   *
   * @throws std::out_of_range If `Empty() == true`.
   * @see Empty()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4, 0xff}; // Sequence: [1, 1, 1, 1]
   * bits.Reset(); // Sequence: [0, 0, 0, 0]
   * bits.Clear(); // Sequence: []
   * bits.Reset(); // Throws std::out_of_range exception
   * @endcode
   */
  constexpr func Reset() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Reset() -> invalid number of bits"};
    }

    FillData(storage_, CalculateCapacity(bits_), BitMask::kReset);
    return *this;
  }

  /**
   * @public
   * @brief Flip bit with given `index` with range checking.
   * @details Stores `true` if bit with given `index` is unset,
   *          otherwise `false` stored. Implies range checking.
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-based index of the bit to flip.
   * @return Reference to `this` object.
   *
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   * @see Size()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4, 0xff}; // Sequence: [1, 1, 1, 1]
   * bits.Flip(0); // Sequence: [0, 1, 1, 1]
   * bits.Flip(0); // Sequence: [1, 1, 1, 1]
   * bits.Flip(100); // Throws std::out_of_range exception
   * @endcode
   */
  constexpr func Flip(
    sizeType index
  ) -> DynamicBitset&
  {
    BITS_DYNAMIC_BITSET_ASSERT(index < bits_);

    if (index >= bits_)
    {
      throw std::out_of_range{std::format("DynamicBitset::Flip(sizeType): index {} >= Size ({})", index, bits_)};
    }

    storage_[index >> BlockInfo::byteDivConst] ^= BitMask::kBit << (index & BlockInfo::byteModConst);
    return *this;
  }

  /**
   * @public
   * @brief Flip all bits in sequence.
   * @ingroup dynamic-bitset-main
   *
   * @return Lvalue reference to `this` object.
   *
   * @throws std::out_of_range If container is empty.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4, 10}; // Sequence: [0, 1, 0, 1]
   * bits.Flip(); // Sequence: [1, 0, 1, 0]
   * bits.Clear(); // Sequence: []
   * bits.Flip(); // Throws std::out_of_range exception
   * @endcode
   */
  constexpr func Flip() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Flip() -> invalid number of bits"};
    }

    pointer end{storage_ + CalculateCapacity(bits_)};

    [[likely]]
    for (pointer begin{storage_}; begin < end; ++begin)
    {
      *begin ^= BitMask::kSet;
    }

    return *this;
  }

  /**
   * @public
   * @brief Swaps the contents of the two bit sequences.
   * @details This method swaps all internal contents of the
   *          two container objects.
   * @ingroup dynamic-bitset-modifiers
   *
   * @param[in] other Another `DynamicBitset` object.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4, 0xff}; // Size() == 4, Sequence: [1, 1, 1, 1]
   * bits::DynamicBitset anotherBits{2}; // Size() == 2, Sequence: [0, 0]
   * bits.Swap(anotherBits); // bits: Size() == 2, Sequence: [0, 0]
   *                         // anotherBits: Size() == 4, Sequence: [1, 1, 1, 1]
   * @endcode
   */
  constexpr func Swap(
    DynamicBitset& other
  ) noexcept -> void
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    std::swap(storage_, other.storage_);
    std::swap(bits_, other.bits_);
    std::swap(blocks_, other.blocks_);
    if constexpr (std::allocator_traits<allocatorType>::propagate_on_container_swap::value)
    {
      std::swap(alloc_, other.alloc_);
    }
    else
    {
      BITS_DYNAMIC_BITSET_ASSERT(alloc_ == other.alloc_);
    }
  }

  /**
   * @public
   * @brief Provides the read/write access to the bit with given index.
   * @details This operator provides the ability to modify or read
   *          bit with given index. Gives fast access with no
   *          range checking.
   * @see BitWrapper.
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-base index of the bit to access.
   * @return `BitWrapper` object to obtain the specified bit.
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined Behaviour** if:
   * - The container is empty `Size() == 0` or `Size() <= index`
   * @see At()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4}; // Sequence: [0, 0, 0, 0]
   * bits[0] = true; // Sequence: [1, 0, 0, 0]
   * bits[10] = true; // Undefined behaviour
   * @endcode
   */
  [[nodiscard]]
  constexpr func operator[](
    sizeType index
  ) noexcept -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return {storage_, static_cast<differenceType>(index)};
  }

  /**
   * @public
   * @brief Provides the read-only access to the bit with given index.
   * @details This operator provides the ability to read bit with given index.
   *          Gives fast access with no range checking.
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-based index of the bit to access.
   * @return `true` if bit is set, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined Behaviour** if:
   * - The container is empty `Size() == 0`.
   * @see At()
   *
   * @par Example:
   * @code{.cpp}
   * const bits::DynamicBitset bits{4}; // Sequence: [0, 0, 0, 0]
   * bool bitValue{bits[0]}; // bitValue == false
   * bool anotherBitValue{bits[10]}; // Undefined behaviour
   * @endcode
   */
  [[nodiscard]]
  constexpr func operator[](
    sizeType index
  ) const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return storage_[index >> BlockInfo::byteDivConst] & BitMask::kBit << (index & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Provides the read/write access to the first bit in bit sequence.
   * @details This method provides the ability to read or modify
   *          state of first bit in sequence. Gives the fast
   *          access to the bit with no range checking.
   * @ingroup dynamic-bitset-access
   *
   * @return `BitWrapper` object to obtain first bit.
   * @see BitWrapper
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined behaviour** if:
   * - The container is empty `Size() == 0`.
   * @see Size()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4}; // Sequence: [0, 0, 0, 0]
   * bits.Front() = true; // Sequence: [1, 0, 0, 0]
   * bits.Clear(); // Sequence: []
   * bits.Front() = false; // Undefined behaviour
   * @endcode
   */
  [[nodiscard]]
  constexpr func Front() noexcept -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return {storage_, 0};
  }

  /**
   * @public
   * @brief Provides the read-only access to the first bit in sequence.
   * @details This method provides the access to the first bit in
   *          bit sequence. Does not modify the container state.
   * @see operator[], At()
   * @ingroup dynamic-bitset-access
   *
   * @return `true` if bit is set, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined behaviour** if:
   * - The container is empty `Size() == 0`.
   * @see Size()
   *
   * @par Example:
   * @code{.cpp}
   * const bits::DynamicBitset bits{4}; // Sequence: [0, 0, 0, 0]
   * bool bitValue{bits.Front()}; // bitValue == false
   * const bits::DynamicBitset anotherBits; // Sequence: []
   * bool anotherBitValue{anotherBits.Front()}; // Undefined behaviour
   * @endcode
   */
  [[nodiscard]]
  constexpr func Front() const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return *storage_ & BitMask::kBit;
  }

  /**
   * @public
   * @brief Provides the read/write access to the last bit in sequence without
   * bounds checking.
   * @details This method provides read and write access to
   *          the last bit in sequence. Gives fast access to
   *          the bit with no range checking.
   * @see BitWrapper
   * @ingroup dynamic-bitset-access
   *
   * @return `BitWrapper` object to obtain last bit.
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined Behaviour** if:
   * - The container is empty `Size() == 0`.
   * @see Size()
   *
   * @note For bounds-checking access, use `At()`.
   * @see At()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{2}; // Sequence: [0, 0]
   * bits.Back() = true; // Sequence: [0, 1]
   * bits.Clear(); // Sequence: []
   * bits.Back() = false; // Undefined behaviour
   * @endcode
   */
  [[nodiscard]]
  constexpr func Back() noexcept -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return {storage_, static_cast<differenceType>(bits_ - 1)};
  }

  /**
   * @public
   * @brief Provides the read-only access to the last bit in sequence without
   * bounds checking.
   * @details This member function provides the access to the last bit in
   *          bit sequence. Does not modify the container state.
   * @ingroup dynamic-bitset-access
   *
   * @return `true` if bit is set, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined Behaviour** if:
   * - The container is empty `Size() == 0`.
   * @see Size()
   *
   * @note For bounds-checked access, use `At()`.
   * @see At()
   *
   * @par Example:
   * @code{.cpp}
   * const bits::DynamicBitset bits{2}; // Sequence: [0, 0]
   * bool bitValue{bits.Back()}; // bitValue = false
   * const bits::DynamicBitset anotherBits; // Sequence: []
   * bool anotherBitValue{anotherBits.Back()}; // Undefined behaviour
   * @endcode
   */
  [[nodiscard]]
  constexpr func Back() const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return storage_[(bits_ - 1) >> BlockInfo::byteDivConst] & BitMask::kBit << ((bits_ - 1) & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Accesses the bit at the specified index with bounds checking.
   * @details This function provides read/write access to a bit in a
   *          sequence using `BitWrapper` class. Unlike `operator[]` and
   *          `Test()`, it performs bounds checking and throws an
   *           exception if the index is invalid.
   * @see BitWrapper
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-based index of the bit to access.
   * @return `BitWrapper` object
   *
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   *
   * @note For unchecked access, use `operator[]` or `Test()`.
   * @see operator[], Test()
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4}; // Sequence: [0, 0, 0, 0]
   * bits.At(0) = true; // Sequence: [1, 0, 0, 0]
   * bits.At(5) = true; // Throws std::out_of_range exception
   * @endcode
   */
  [[nodiscard]]
  constexpr func At(
    sizeType index
  ) -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(index < bits_);

    if (index >= bits_)
    {
      throw std::out_of_range{std::format("bits::DynamicBitset::At(sizeType): index {} >= Size ({})", index, bits_)};
    }

    return {storage_, static_cast<differenceType>(index)};
  }

  /**
   * @public
   * @brief Accesses the bit at the specified index with bounds checking.
   * @details This function provides read-only access to a bit
   *          in the sequence. Unlike `operator[]` and `Test()`, it performs
   *          bounds checking and throws an exception if the index is invalid.
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-based index of the bit to access.
   * @return `true` if the bit is set, `false` otherwise.
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   *
   * @note For unchecked access, use `operator[]` or `Test()`.
   * @see operator[], Test()
   *
   * @par Example:
   * @code{.cpp}
   * const bits::DynamicBitset bits{2}; // Sequence: [0, 0]
   * bool bitValue{bits.At(0)}; // bitValue == false
   * const bits::DynamicBitset anotherBits; // Sequence: []
   * bool anotherBitValue{anotherBits.At(0)}; // Throws std::out_of_range
   * exception
   * @endcode
   */
  [[nodiscard]]
  constexpr func At(
    sizeType index
  ) const -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(index < bits_);

    if (index >= bits_)
    {
      throw std::out_of_range{
        std::format("bits::DynamicBitset::At(sizeType) const: index {} >= Size ({})", index, bits_)
      };
    }

    return storage_[index >> BlockInfo::byteDivConst] & BitMask::kBit << (index & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Accesses the bit at the specified index.
   * @details This function provides read-only access to the bit
   *          in the sequence. It does not modify the container and
   *          is equivalent to `operator[] const noexcept`.
   * @ingroup dynamic-bitset-access
   *
   * @param[in] index The zero-based index of the bit to access.
   * @return `true` if the bit is set, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @warning **Undefined Behaviour** if:
   * - `index >= Size()` (out-of-bounds access).
   * - The container is empty (`Size() == 0`).
   *
   * @note For bounds-checked access, use `At()`.
   * @see At()
   *
   * @par Example:
   * @code{.cpp}
   * @endcode
   */
  [[nodiscard]]
  constexpr func Test(
    sizeType index
  ) const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return storage_[index >> BlockInfo::byteDivConst] & BitMask::kBit << (index & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Copies the contents of another bit sequence.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other Another `DynamicBitset` object.
   * @return Reference to `this` object.
   *
   * @throws std::bad_alloc If reallocation of internal buffer takes place.
   *
   * @note Complexity: O(n), where n is the block count -> \f$ n = $lceil
   * $frac{bitsCount}{sizeof(blockType) * bitsPerByte} $rceil \f$
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{8, 0xff}; // Size() == 8, Sequence: [1, 1, 1, 1,
   * 1, 1, 1, 1] bits::DynamicBitset anotherBits; // Size() == 0, Sequence: []
   * anotherBits = bits; // Size() == 8, Sequence: [1, 1, 1, 1, 1, 1, 1, 1]
   * @endcode
   */
  constexpr func operator=(
    const DynamicBitset& other
  ) /* clang-format off */ -> DynamicBitset& /* clang-format on */
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    if (blocks_ != other.blocks_)
    {
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
      if constexpr (std::allocator_traits<allocatorType>::propagate_on_container_copy_assignment::value)
      {
        storage_ = std::allocator_traits<allocatorType>::allocate(other.alloc_, other.blocks_);
      }
      else
      {
        storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, other.blocks_);
      }
    }

    CopyData(other.storage_, storage_, CalculateCapacity(other.blocks_));
    bits_ = other.bits_;
    blocks_ = other.blocks_;
    if constexpr (std::allocator_traits<allocatorType>::propagate_on_container_copy_assignment::value)
    {
      alloc_ = other.alloc_;
    }

    return *this;
  }

  /**
   * @public
   * @brief Moves the contents of `other` to `this` object
   * @details `this` object clears it's state and then occupies the contents of
   * `other` object.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other Another `DynamicBitset` object.
   * @return Lvalue reference to `this` object.
   *
   * @note Complexity: O(1)
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{4}; // Size() == 4, Sequence: [0, 0, 0, 0]
   * bits::DynamicBitset b; // Size() == 0, Sequence: []
   * b = std::move(a);
   * // a: Size() == 0, Sequence: []
   * // b: Size() == 4, Sequence: [0, 0, 0, 0]
   * @endcode
   */
  constexpr func operator=(
    DynamicBitset&& other
  ) noexcept -> DynamicBitset&
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    if (storage_ != nullptr)
    {
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, blocks_);
      storage_ = nullptr;
      bits_ = blocks_ = 0;
    }

    std::swap(storage_, other.storage_);
    std::swap(bits_, other.bits_);
    std::swap(blocks_, other.blocks_);
    if constexpr (std::allocator_traits<allocatorType>::propagate_on_container_move_assignment::value)
    {
      alloc_ = std::move(other.alloc_);
    }

    return *this;
  }

  /**
   * @public
   * @brief Performs bitwise AND operation on all bits.
   * @details This operator performs bitwise AND operation
   *          with similar semantics as builtin integral types.
   * @ingroup dynamic-bitset-bitops
   *
   * @param[in] other Another `DynamicBitset` object.
   * @return Lvalue reference to `this` object.
   *
   * @throws `std::invalid_argument` if one condition satisfied:
   * - `this` object is empty;
   * - `other` object is empty;
   * - `this` object size is not equal to `other` object size.
   *
   * @note Compilexity: O(n), where n is the block count -> \f$ n = \lceil
   * \frac{bitsCount}{sizeof(blockType) * bitsPerByte} \rceil \f$
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{4, 0b1111}; // Size() == 4, Sequence: [1, 1, 1, 1]
   * bits::DynamicBitset b{4}; // Size() == 4, Sequence: [0, 0, 0, 0]
   * a &= b;
   * // a: Size() == 4, Sequence: [0, 0, 0, 0]
   * @endcode
   */
  constexpr func operator&=(
    const DynamicBitset& other
  ) /* clang-format off */ -> DynamicBitset& /* clang-format on */
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    if (bits_ != other.bits_ || !bits_ || !other.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator&= -> invalid storage size"};
    }

    pointer beginRhs{other.storage_};
    pointer end{storage_ + CalculateCapacity(bits_)};

    [[likely]]
    for (pointer beginLhs{storage_}; beginLhs < end; ++beginLhs)
    {
      *beginLhs &= *beginRhs++;
    }

    return *this;
  }

  /**
   * @public
   * @brief Performs bitwise OR operation on all bits.
   * @details This operator performs bitwise OR operation
   *          with similar semantics as builtin integral types.
   * @ingroup dynamic-bitset-bitops
   *
   * @param[in] other Another `DynamicBitset` object.
   * @return Lvalue reference to `this` object.
   *
   * @throws `std::invalid_argument` if one condition is satisfied:
   * - `this` object is empty;
   * - `other` object is empty;
   * - `this` object size is not equal to `other` object size.
   *
   * @note Compilexity: O(n), where n is the block count -> \f$ n = \lceil
   * \frac{bitsCount}{sizeof(blockType) * bitsPerByte} \rceil \f$
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{4}; // Size() == 4, Sequence: [0, 0, 0, 0]
   * bits::DynamicBitset b{4, 0b1111}; // Size() == 4, Sequence: [1, 1, 1, 1]
   * a |= b;
   * // a: Size() == 4, Sequence: [1, 1, 1, 1]
   * @endcode
   */
  constexpr func operator|=(
    const DynamicBitset& other
  ) /* clang-format off */ -> DynamicBitset& /* clang-format on */
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    if (bits_ != other.bits_ || !bits_ || !other.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator|= -> invalid storage size"};
    }

    pointer beginSelf{storage_};
    pointer beginOther{other.storage_};
    pointer end{storage_ + CalculateCapacity(bits_)};

    [[likely]]
    while (beginSelf < end)
    {
      *beginSelf++ |= *beginOther++;
    }

    return *this;
  }

  /**
   * @public
   * @brief Performs bitwise XOR operation on all bits.
   * @details This operator performs bitwise XOR operation
   *          with similar semantics as builtin integral types.
   * @ingroup dynamic-bitset-bitops
   *
   * @param[in] other Another `DynamicBitset` object.
   * @return Lvalue reference to `this` object.
   *
   * @throws `std::invalid_argument` if one condition is satisfied:
   * - `this` object is empty;
   * - `other` object is empty;
   * - `this` object size is not equal to `other` object size.
   *
   * @note Compilexity: O(n), where n is the block count -> \f$ n = \lceil
   * \frac{bitsCount}{sizeof(blockType) * bitsPerByte} \rceil \f$
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{4}; // Size() == 4, Sequence: [0, 0, 0, 0]
   * bits::DynamicBitset b{4, 0b1111}; // Size() == 4, Sequence: [1, 1, 1, 1]
   * a ^= b;
   * // a: Size() == 4, Sequence: [1, 1, 1, 1]
   * @endcode
   */
  constexpr func operator^=(
    const DynamicBitset& other
  ) /* clang-format off */ -> DynamicBitset& /* clang-format on */
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other && bits_ == other.bits_);

    if (bits_ != other.bits_ || !bits_ || !other.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator^= -> invalid storage size"};
    }

    pointer beginLhs{storage_};
    pointer beginOther{other.storage_};
    pointer end{storage_ + blocks_};

    [[likely]]
    while (beginLhs < end)
    {
      *beginLhs++ ^= *beginOther++;
    }

    return *this;
  }

  /**
   * @public
   * @brief Performs bitwise inversion on all bits.
   * @details This operator performs bitwise inverse operation
   *          with similar semantics as builtin integral types.
   * @ingroup dynamic-bitset-bitops
   *
   * @return New `DynamicBitset` copy with all reversed bits.
   *
   * @throws std::out_of_range If the container is empty `Empty() == true`.
   *
   * @note Complexity: O(n), where n is the block count -> \f$ n = \lceil
   * \frac{bitsCount}{sizeof(blockType) * bitsPerByte} \rceil \f$
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{4}; // Size() == 4, Sequence: [0, 0, 0, 0]
   * auto b{~a}; // Size() == 4, Sequence: [1, 1, 1, 1]
   * @endcode
   */
  [[nodiscard]]
  constexpr func operator~() const
    /* clang-format off */ -> DynamicBitset /* clang-format on */
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    if (!storage_)
    {
      throw std::out_of_range{"DynamicBitset::operator~ -> invalid storage pointer (nullptr)"};
    }

    auto bits{*this};
    pointer end{bits.storage_ + CalculateCapacity(bits_)};

    [[likely]]
    for (pointer begin{bits.storage_}; begin < end; ++begin)
    {
      *begin ^= BitMask::kSet;
    }

    return bits;
  }

  /**
   * @public
   * @brief Performs logical right bitwise shift on `DynamicBitset`.
   * @details This operator performs logical right bitwise shift operation
   *          with similiar semantics as builtin unsigned integral types.
   * @ingroup dynamic-bitset-bitops
   *
   * @param[in] bitOffset Unsigned integral value representing bitwise shift
   * offset.
   * @return Lvalue reference to `this` object.
   *
   * @throws std::out_of_range If the container is empty `Empty() == true`.
   *
   * @note Complexity: O(n), where n is the number of bits.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{8, 0xff}; // Sequence: [1, 1, 1, 1, 1, 1, 1, 1]
   * bits >>= 4; // Sequence: [0, 0, 0, 0, 1, 1, 1, 1]
   * bits.Set(); // Sequence: [1, 1, 1, 1, 1, 1, 1, 1]
   * bits >>= 8; // Sequence: [0, 0, 0, 0, 0, 0, 0, 0]
   * @endcode
   */
  constexpr func operator>>=(
    sizeType bitOffset
  ) /* clang-format off */ -> DynamicBitset& /* clang-format on */
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::operator>>= -> invalid storage pointer (nullptr)"};
    }
    else if (bitOffset >= bits_)
    {
      FillData(storage_, CalculateCapacity(bits_), BitMask::kReset);
    }
    else if (bitOffset)
    {
      sizeType totalShifts{bitOffset - 1};

      for (sizeType bit{bits_ - 1}; bit > totalShifts; --bit)
      {
        sizeType bitShift{bit - bitOffset};
        sizeType byte{
          storage_[bitShift >> BlockInfo::byteDivConst] & BitMask::kBit << (bitShift & BlockInfo::byteModConst)
        };
        SetBit(bit, byte);
      }

      FillData(storage_, bitOffset >> BlockInfo::byteDivConst, BitMask::kReset);

      for (sizeType bit{bitOffset - (bitOffset & BlockInfo::byteModConst)}; bit != bitOffset; ++bit)
      {
        SetBit(bit, false);
      }
    }

    return *this;
  }

  /**
   * @public
   * @brief Performs logical left bitwise shift on `DynamicBitset`.
   * @details This operator performs logical left bitwise shift operation
   *          with similiar semantics as builtin unsigned integral types.
   * @ingroup dynamic-bitset-bitops
   *
   * @param[in] bitOffset Unsigned integral value representing bitwise shift
   * offset.
   * @return Lvalue reference to `this` object.
   *
   * @throws std::out_of_range If the container is empty `Empty() == true`.
   * @see Empty()
   *
   * @note Complexity: O(n), where n is the number of bits.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{8, 0xff}; // Sequence: [1, 1, 1, 1, 1, 1, 1, 1]
   * bits <<= 4; // Sequence: [1, 1, 1, 1, 0, 0, 0, 0]
   * bits.Set(); // Sequence: [1, 1, 1, 1, 1, 1, 1, 1]
   * bits <<= 8; // Sequence: [0, 0, 0, 0, 0, 0, 0, 0]
   * @endcode
   */
  constexpr func operator<<=(
    sizeType bitOffset
  ) /* clang-format off */ -> DynamicBitset& /* clang-format on */
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::operator<<= -> invalid storage pointer (nullptr)"};
    }
    else if (bitOffset >= bits_)
    {
      FillData(storage_, CalculateCapacity(bits_), BitMask::kReset);
    }
    else if (bitOffset)
    {
      sizeType totalShifts{bits_ - bitOffset};
      for (sizeType bit{}; bit < totalShifts; ++bit)
      {
        sizeType bitShift{bit + bitOffset};
        sizeType state{
          storage_[bitShift >> BlockInfo::byteDivConst] & BitMask::kBit << (bitShift & BlockInfo::byteModConst)
        };
        SetBit(bit, state);
      }
      sizeType byteShift{bitOffset >> BlockInfo::byteDivConst};
      FillData(storage_ + (CalculateCapacity(bits_) - byteShift), byteShift, BitMask::kReset);
      sizeType bitShift{bits_ - (byteShift << 3UL)};
      for (sizeType bit{bits_ - bitOffset}; bit != bitShift; ++bit)
      {
        SetBit(bit, false);
      }
    }

    return *this;
  }

  /**
   * @public
   * @brief Performs bitwise comparison between two `DynamicBitset` objects.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other Other `DynamicBitset` object.
   * @return `true` If `lhs` bits is equal to `rhs`, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @note Complexity: O(n), where n is the block count -> \f$ n = \lceil
   * \frac{bitsCount}{sizeof(blockType) * bitsPerByte} \rceil \f$
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4, 0xff};
   * bits::DynamicBitset anotherBits{bits};
   * assert(bits == anotherBits);
   * @endcode
   */
  [[nodiscard]]
  constexpr func operator==(
    const DynamicBitset& other
  ) const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    if (bits_ != other.bits_)
    {
      return false;
    }
    pointer firstSelf{storage_};
    pointer firstOther{other.storage_};
    pointer end{storage_ + CalculateCapacity(bits_) - 1};
    [[likely]]
    while (firstSelf < end)
    {
      if (*firstSelf++ != *firstOther++)
      {
        return false;
      }
    }
    const blockType remainingBits{static_cast<blockType>(bits_ & BlockInfo::byteModConst)};
    for (blockType currentBit{1}; currentBit <= remainingBits; currentBit <<= 1)
    {
      if (!((*firstSelf & currentBit) & *firstOther))
      {
        return false;
      }
    }
    return true;
  }

  /**
   * @public
   * @brief Performs bitwise comparison between two `DynamicBitset` objects.
   * @ingroup dynamic-bitset-main
   *
   * @param[in] other Other `DynamicBitset` object.
   * @return `true` If `this` bitset is not equal to `other`, `false` otherwise.
   *
   * @throws None (no-throw guarantee).
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{16, 0xff};
   * bits::DynamicBitset b{std::move(a)};
   * assert(a != b);
   * @endcode
   */
  [[nodiscard]]
  constexpr func operator!=(
    const DynamicBitset& other
  ) const noexcept -> bool
  {
    return !(*this == other);
  }

  /**
   * @public
   * @brief Converts the bitset to a human-readable string representation.
   * @ingroup dynamic-bitset-format
   *
   * @return A new `std::string` where each character represents a bit:
   * - '1' for set bits.
   * - '0' for unset bits.
   * - Character order matches bit indices (`strBits[0] == bits[0]).
   *
   * @throws std::bad_alloc If memory allocation during `std::string`
   * construction fails.
   *
   * @note Characteristics:
   * - Output length exactly equals `Size()`.
   * - Bit order: LSB (bit 0) -> MSB (bit N - 1) by default.
   *
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{4};
   * bits.Flip(0);
   * bits.Set(2, true);
   * auto s{bits.ToString()}; // '1010'
   * @endcode
   */
  [[nodiscard]]
  constexpr func ToString() const -> std::string
  {
    std::string strBits(bits_, '\0');

    for (sizeType currentBit{}; currentBit < bits_; ++currentBit)
    {
      strBits[currentBit] =
        (storage_[currentBit >> BlockInfo::byteDivConst] >> (currentBit & BlockInfo::byteModConst) & BitMask::kBit) |
        '0';
    }

    return strBits;
  }

 private:
  pointer storage_{nullptr};
  sizeType bits_{};
  sizeType blocks_{};
  [[no_unique_address]]
  allocatorType alloc_;
};

}  // namespace bits

/**
 * @brief Performs a bitwise AND between two `DynamicBitset` objects.
 * @details Returns a new `DynamicBitset` where each bit is the result of:
 *          `lhs[i] & rhs[i]` for all positions.
 *          Delegates to operator&= internally; see its documentation for
 *          AND semantics.
 * @see operator&=
 * @ingroup dynamic-bitset-bitops
 *
 * @tparam Block Unsigned integral type for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 *
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @return New `DynamicBitset` containing the AND result.
 *
 * @throws std::bad_alloc If memory allocation fails during copy construction
 * (std::allocator).
 * @note This is non-modifying operation (original `DynamicBitset` objects
 * remains unchanged).
 */
template<typename Block, typename Alloc>
[[nodiscard]]
constexpr func operator&(
  const bits::DynamicBitset<Block, Alloc>& lhs,  //
  const bits::DynamicBitset<Block, Alloc>& rhs
) /* clang-format off */ -> bits::DynamicBitset<Block, Alloc> /* clang-format on */
{
  BITS_DYNAMIC_BITSET_ASSERT(&lhs != &rhs);

  auto bits{lhs};
  bits &= rhs;
  return bits;
}

/**
 * @brief Performs a bitwise OR between two `DynamicBitset` objects.
 * @details Returns a new `DynamicBitset` where each bit is the result of:
 *          `lhs[i] | rhs[i]` for all positions.
 *          Delegates to operator|= internally; see its documentation for
 *          OR semantics.
 * @see operator|=
 * @ingroup dynamic-bitset-bitops
 *
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 *
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @return New `DynamicBitset` containing the OR result.
 *
 * @throws std::bad_alloc If memory allocation fails during copy construction
 * (std::allocator).
 * @note This is non-modifying operation (original `DynamicBitset` objects
 * remains unchanged).
 */
template<typename Block, typename Alloc>
[[nodiscard]]
constexpr func operator|(
  const bits::DynamicBitset<Block, Alloc>& lhs,  //
  const bits::DynamicBitset<Block, Alloc>& rhs
) /* clang-format off */ -> bits::DynamicBitset<Block, Alloc> /* clang-format on */
{
  BITS_DYNAMIC_BITSET_ASSERT(&lhs != &rhs);

  auto bits{lhs};
  bits |= rhs;
  return bits;
}

/**
 * @brief Performs a bitwise XOR between two `DynamicBitset` objects.
 * @details Returns a new `DynamicBitset` where each bit is the result of:
 *          `lhs[i] ^ rhs[i]` for all positions.
 *          Delegates to operator^= internally; see its documentation for
 *          XOR semantics.
 * @see operator^=
 * @ingroup dynamic-bitset-bitops
 *
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 *
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @return New `DynamicBitset` containing the XOR result.
 *
 * @throws std::bad_alloc If memory allocation fails during copy construction
 * (std::allocator).
 * @note This is non-modifying operation (original `DynamicBitset` objects
 * remains unchanged).
 */
template<typename Block, typename Alloc>
[[nodiscard]]
constexpr func operator^(
  const bits::DynamicBitset<Block, Alloc>& lhs,  //
  const bits::DynamicBitset<Block, Alloc>& rhs
) /* clang-format off */ -> bits::DynamicBitset<Block, Alloc> /* clang-format on */
{
  BITS_DYNAMIC_BITSET_ASSERT(&lhs != &rhs);

  auto bits{lhs};
  bits ^= rhs;
  return bits;
}

/**
 * @brief Returns a copy of the `DynamicBitset` object left-shifted by
 * `bitOffset` positions.
 * @details This operation applies logical bitwise shift to the sequence.
 *          If `bitOffset >= Size()` all bits will be unset, otherwise pretends
 *          as a builtin operator for fundamental unsigned integral types.
 * @ingroup dynamic-bitset-bitops
 *
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 *
 * @param[in] bits Original `DynamicBitset` object to shift.
 * @param[in] bitOffset Number of positions to shift (>= 0).
 * @return New `DynamicBitset` object with shifted bits.
 *
 * @throws std::bad_alloc If memory allocation fails during copy construction
 * (std::allocator).
 * @note This is non-modifying operation (original `DynamicBitset` object
 * remains unchanged).
 * @note Delegates to `operator<<=` internally; see its documentation for shift
 * semantics.
 * @see operator<<=
 */
template<typename Block, typename Alloc>
[[nodiscard]]
constexpr func operator<<(
  const bits::DynamicBitset<Block, Alloc>& bits,  //
  const size_t bitOffset
) /* clang-format off */ -> bits::DynamicBitset<Block, Alloc> /* clang-format on */
{
  auto tempBits{bits};
  tempBits <<= bitOffset;
  return tempBits;
}

/**
 * @brief Returns a copy of the `DynamicBitset` object right-shifted by
 * `bitOffset` positions.
 * @details This operation applies logical bitwise shift to the sequence.
 *          If `bitOffset >= Size()` all bits will be unset, otherwise pretends
 *          as a builtin operator for fundamental unsigned integral types.
 * @ingroup dynamic-bitset-bitops
 *
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 *
 * @param[in] bits Original `DynamicBitset` object to shift.
 * @param[in] bitOffset Number of positions to shift (>= 0).
 * @return New `DynamicBitset` object with shifted bits.
 *
 * @throws std::bad_alloc If memory allocation fails during copy construction
 * (std::allocator).
 * @note This is non-modifying operation (original `DynamicBitset` object
 * remains unchanged).
 * @note Delegates to `operator>>=` internally; see its documentation for shift
 * semantics.
 * @see operator>>=
 */
template<typename Block, typename Alloc>
[[nodiscard]]
constexpr func operator>>(
  const bits::DynamicBitset<Block, Alloc>& bits,  //
  const size_t bitOffset
) /* clang-format off */ -> bits::DynamicBitset<Block, Alloc> /* clang-format on */
{
  auto tempBits{bits};
  tempBits >>= bitOffset;
  return tempBits;
}

/**
 * @namespace std
 */
namespace std
{

/**
 * @brief Enables `std::format` support for `DynamicBitset`.
 * @ingroup dynamic-bitset-format
 *
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 */
template<typename Block, typename Alloc>
struct formatter<bits::DynamicBitset<Block, Alloc>> : public formatter<string>
{
  [[nodiscard]]
  func format(
    const bits::DynamicBitset<Block, Alloc> bits,  //
    format_context& ctx
  ) const
    /* clang-format off */ -> decltype(formatter<string>::format(bits.ToString(), ctx)) /* clang-format on */
  {
    return formatter<string>::format(bits.ToString(), ctx);
  }
};

/**
 * @brief Exchanges the contents of two `DynamicBitset` objects.
 *
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 *
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBItset` operand.
 *
 * @note All elements and allocators (if propogate_on_container_swap is not
 * equal to std::false_type) are exchanged between `lhs` and `rhs`.
 * @throws None (no-throw guarantee).
 *
 * @par Example:
 * @code{.cpp}
 * bits::DynamicBitset a{10};
 * bits::DynamicBitset b{100};
 * std::swap(a, b); // a.Size() == 100, b.Size() == 10
 * @endcode
 */
template<typename Block, typename Alloc>
constexpr func swap(
  bits::DynamicBitset<Block, Alloc>& lhs,  //
  bits::DynamicBitset<Block, Alloc>& rhs
) noexcept -> void
{
  lhs.Swap(rhs);
}

}  // namespace std

#undef func
#if defined(BITS_FUNC_MACRO_REDEFINED)
  #undef BITS_FUNC_MACRO_REDEFINED
  #pragma pop_macro("func")
#endif