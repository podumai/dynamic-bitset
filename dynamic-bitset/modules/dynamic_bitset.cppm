module;

#include <bit>
#include <climits>
#include <concepts>
#include <cstdint>
#include <format>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cassert>

#if CHAR_BIT != 8
  #error "bits::DynamicBitset supports platforms with 8 bits per byte."
#endif

export module dynamic_bitset;

#define BEGIN_EXPORT_MODULE_SECTION \
export \
{
#define END_EXPORT_MODULE_SECTION }

#define BEGIN_BITS_NAMESPACE \
namespace bits \
{
#define END_BITS_NAMESPACE }

#define func auto

#define BITS_DYNAMIC_BITSET_ASSERT(...)

/**
* @concept IsValidDynamicBitsetBlockType
*/
template <typename Block>
concept IsValidDynamicBitsetBlockType =
   std::unsigned_integral<Block> && !std::is_same_v<Block, bool> && !(sizeof(Block) & (sizeof(Block) - 1));

/**
* @concept IsValidDynamicBitsetAllocType
*/
template <typename Alloc>
concept IsValidDynamicBitsetAllocType = std::unsigned_integral<typename std::allocator_traits<Alloc>::value_type> &&
                                       !std::is_same_v<typename std::allocator_traits<Alloc>::value_type, bool> &&
                                       !(sizeof(typename std::allocator_traits<Alloc>::value_type) &
                                         (sizeof(typename std::allocator_traits<Alloc>::value_type) - 1));

/**
* @concept IsValidDynamicBitsetBlockIterator
*/
template <typename BlockIterator, typename TargetBlock>
concept IsValidDynamicBitsetBlockIterator = std::is_convertible_v<decltype(*BlockIterator{}), TargetBlock>;

BEGIN_EXPORT_MODULE_SECTION

BEGIN_BITS_NAMESPACE

template<
    IsValidDynamicBitsetBlockType Block,
    IsValidDynamicBitsetAllocType Alloc
        >
class DynamicBitset;

template<typename Alloc = std::allocator<size_t>>
DynamicBitset() -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>;

template<typename Alloc = std::allocator<size_t>>
DynamicBitset(
    typename std::allocator_traits<Alloc>::size_type
) -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>;

template<typename Alloc = std::allocator<size_t>>
DynamicBitset(
    typename std::allocator_traits<Alloc>::size_type,
    typename std::allocator_traits<Alloc>::value_type
) -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>;

template<typename Alloc = std::allocator<size_t>>
DynamicBitset(
    typename std::allocator_traits<Alloc>::size_type,
    typename std::allocator_traits<Alloc>::value_type,
    const Alloc&
) -> DynamicBitset<typename std::allocator_traits<Alloc>::value_type, Alloc>;

/**
 * @class DynamicBitset
 * @tparam Block Unsigned integral type used for bit storage.
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 */
template<
    IsValidDynamicBitsetBlockType Block = size_t,
    IsValidDynamicBitsetAllocType Alloc = std::allocator<Block>
        >
class DynamicBitset
{
 public:
  class Iterator;
  class ConstIterator;

 public:
  using block_type = Block;
  using blockType = Block;
  using allocator_type = Alloc;
  using allocatorType = Alloc;
  using size_type = typename std::allocator_traits<allocator_type>::size_type;
  using sizeType = typename std::allocator_traits<allocatorType>::size_type;
  using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
  using differenceType = typename std::allocator_traits<allocatorType>::difference_type;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer = const typename std::allocator_traits<allocator_type>::pointer;
  using constPointer = const typename std::allocator_traits<allocatorType>::pointer;
  using value_type = bool;
  using valueType = bool;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

 private:
  enum bitMask : blockType
  {
    BIT = 1,
    RESET = 0,
    SET = static_cast<blockType>(-1)
  };

  struct BlockInfo final
  {
    static constexpr sizeType bitsCount{std::numeric_limits<blockType>::digits};
    static constexpr sizeType byteDivConst{bitsCount < 32 ? sizeof(blockType) + 2 : bitsCount == 32 ? 5 : 6};
    static constexpr sizeType byteModConst{bitsCount - 1};
  };

 public:
  /**
   * @class Iterator
   * @ingroup bitset-iterators
   * @brief Random-access iterator for `DynamicBitset`.
   * @public
   */
  class Iterator 
  {
   private:
    friend DynamicBitset;

   public:
    using difference_type = typename DynamicBitset::difference_type;
    using differenceType = typename DynamicBitset::differenceType;
    using pointer = typename DynamicBitset::pointer;
    using value_type = typename DynamicBitset::value_type;
    using valueType = typename DynamicBitset::valueType;
    using iterator_category = typename std::contiguous_iterator_tag;
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
          pointer ptr,
          differenceType bitPosition
      ) noexcept
          : byte_{ptr}
          , bit_{bitPosition}
      {
        /* Empty */
      }

      constexpr BitWrapper(const BitWrapper& other) noexcept = default;
      constexpr ~BitWrapper() = default;

      constexpr func operator=(bool value) noexcept -> BitWrapper&
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        if (value)
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] |=
          DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
        }
        else
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &=
          ~(DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst));
        }

        return *this;
      }

     private:
      [[nodiscard]] constexpr func GetBit() const noexcept -> bool
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        return byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &
          DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
      }

      constexpr func SetBit() noexcept -> void
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] |=
          DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
      }

      constexpr func UnsetBit() noexcept -> void
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &=
          ~(DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst));
      }

     public:
      constexpr func operator=(const BitWrapper& other) noexcept -> BitWrapper&
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

      constexpr func operator|=(bool value) noexcept -> BitWrapper&
      {
        if (value)
        {
          SetBit();
        }
        return *this;
      }

      constexpr func operator&=(bool value) noexcept -> BitWrapper&
      {
        if (!value)
        {
          UnsetBit();
        }
        return *this;
      }

      constexpr func operator^=(bool value) noexcept -> BitWrapper&
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

        if (value)
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] ^=
            DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
        }
        return *this;
      }

      [[nodiscard]] constexpr func operator==(const BitWrapper& other) const noexcept -> bool
      {
        BITS_DYNAMIC_BITSET_ASSERT(byte_ == other.byte_);

        return bit_ == other.bit_;
      }

      [[nodiscard]] constexpr func operator!=(const BitWrapper& other) const noexcept -> bool
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
     * @throws None (no-throw guarantee).
     */
    constexpr Iterator() noexcept = default;

    constexpr Iterator(
        pointer ptr, 
        differenceType bitPosition
    ) noexcept
        : wrapper_{ptr, bitPosition}
    {
      /* Empty */
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr Iterator(const Iterator&) noexcept = default;
    constexpr ~Iterator() = default;

    /**
     * @public
     * @brief Exchanges the contents of this iterator with another.
     * @param[in] other Iterator to swap with.
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
    constexpr func Swap(Iterator& other) noexcept -> void
    {
      BITS_DYNAMIC_BITSET_ASSERT(this != &other);

      wrapper_.Swap(other.wrapper_);
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator++() noexcept -> Iterator&
    {
      ++wrapper_.bit_;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator++(int) noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_++};
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator+=(differenceType index) noexcept -> Iterator&
    {
      wrapper_.bit_ += index;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator+(differenceType index) const noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_ + index};
    }

    /**
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator+(differenceType index, const Iterator& iter) noexcept -> Iterator
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
     * @throws None (no-throw guarantee).
     */
    constexpr func operator--(int) noexcept -> Iterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ > 0);

      return {wrapper_.byte_, wrapper_.bit_--};
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator-=(differenceType index) noexcept -> Iterator&
    {
      wrapper_.bit_ -= index;

      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ >= 0);

      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator-(differenceType index) const noexcept -> Iterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.bit_ - index >= 0);

      return {wrapper_.byte_, wrapper_.bit_ - index};
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator-(const Iterator& other) const noexcept -> differenceType
    {
      BITS_DYNAMIC_BITSET_ASSERT(wrapper_.byte_ == other.wrapper_.byte_);

      return wrapper_.bit_ - other.wrapper_.bit_;
    }

    /**
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator<(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ < rhs.wrapper_.bit_;
    }

    /**
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator<=(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ <= rhs.wrapper_.bit_;
    }

    /**
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator>(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ > rhs.wrapper_.bit_;
    }

    /**
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator>=(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ >= rhs.wrapper_.bit_;
    }

    /**
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator==(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_.byte_);

      return lhs.wrapper_.bit_ == rhs.wrapper_.bit_;
    }

    /**
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator!=(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.wrapper_.byte_ == rhs.wrapper_byte_);

      return lhs.wrapper_.bit_ != rhs.wrapper_.bit_;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator*() noexcept -> BitWrapper&
    {
      return wrapper_;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator[](differenceType index) noexcept -> BitWrapper
    {
      return {wrapper_.byte_, wrapper_.bit_ + index};
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr Iterator& operator=(const Iterator&) noexcept = default;

   private:
    BitWrapper wrapper_;
  };

  /**
   * @public
   * @class ConstIterator
   * @ingroup dynamic-bitset-iterators
   * @brief Random-access read-only iterator for `DynamicBitset`.
   * 
   * @details Supports:
   *  - Unary prefix/postfix increment/decrement operators (`++it`, `it++`, `--it`, `it--`).
   *  - Unary dereference operator (`*it` returns `true` if bit is set, `false` otherwise).
   *  - Binary comparison operators (`it1 == it2`, `it1 != it2`, `it1 > it2`, `it1 >= it2`, `it1 < it2`, `it1 <= it2`).
   *  - Binary arithmetic operators (`it += offset`, `it -= offset`, `it + offset`, `offset + it`, `it - offset`, `it[offset]`).
   * @see operator++, operator--
   * @see operator*
   * @see operator==, operator!=, operator>, operator>=, operator<, operator<=
   * @see operator+=, operator-=, operator+, operator-, operator[]
   * 
   * @warning Invalidates 
   */
  class ConstIterator
  {
   private:
    friend DynamicBitset;
 
   public:
    using difference_type = typename DynamicBitset::difference_type;
    using differenceType = typename DynamicBitset::differenceType;
    using value_type = typename DynamicBitset::value_type;
    using valueType = typename DynamicBitset::valueType;
    using pointer = typename DynamicBitset::pointer;
    using iterator_category = typename std::contiguous_iterator_tag;
    using iteratorCategory = typename std::contiguous_iterator_tag;

   public:
    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr ConstIterator() noexcept = default;
  
    constexpr ConstIterator(
        pointer ptr,
        differenceType bit_position
    ) noexcept
        : byte_{ptr}
        , bit_{bit_position}
    {
      /* Empty */
    }

    /**
     * @public
     * @param[in] other.
     * @throws None (no-throw guarantee).
     */
    constexpr ConstIterator(const ConstIterator& other) noexcept = default;

    constexpr ~ConstIterator() = default;

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator++() noexcept -> ConstIterator&
    {
      ++bit_;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator++(int) noexcept -> ConstIterator
    {
      return {byte_, bit_++};
    }

    /**
     * @public
     * @public
     * @typedef 
     *
     * @param[in] index.
     * @throws None (no-throw guarantee).
     */
    constexpr func operator+=(differenceType index) noexcept -> ConstIterator&
    {
      bit_ += index;
      return *this;
    }

    /**
     * @public
     * @param[in] index.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator+(differenceType index) noexcept -> ConstIterator
    {
      return {byte_, bit_ + index};
    }

    /**
     * @public
     * @param[in] index.
     * @param[in] iter.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator+(
        differenceType index,
        const ConstIterator& iter
    ) noexcept -> ConstIterator
    {
      return {iter.byte_, iter.bit_ + index};
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    constexpr func operator--() noexcept -> ConstIterator&
    {
      BITS_DYNAMIC_BITSET_ASSERT(bit_ > 0);

      --bit_;
      return *this;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator--(int) noexcept -> ConstIterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(bit_ > 0);

      return {byte_, bit_--};
    }

    /**
     * @public
     * @param[in] index.
     * @throws None (no-throw guarantee).
     */
    constexpr func operator-=(differenceType index) noexcept -> ConstIterator&
    {
      bit_ -= index;

      BITS_DYNAMIC_BITSET_ASSERT(bit_ >= 0);

      return *this;
    }

    /**
     * @public
     * @param[in] index.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator-(differenceType index) noexcept -> ConstIterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(bit_ - index >= 0);

      return {byte_, bit_ - index};
    }

    /**
     * @param[in] index.
     * @param[in] other.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator-(
        differenceType index,
        const ConstIterator& other
    ) noexcept -> ConstIterator
    {
      BITS_DYNAMIC_BITSET_ASSERT(other.bit_ - index >= 0);

      return {other.byte_, other.bit_ - index};
    }

    /**
     * @public
     * @param[in] other.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator-(const ConstIterator& other) noexcept -> differenceType
    {
      return bit_ - other.bit_;
    }

    /**
     * @param[in] lhs.
     * @param[in] rhs.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator>(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.byte_ == rhs.byte_);

      return lhs.bit_ > rhs.bit_;
    }

    /**
     * @param[in] lhs.
     * @param[in] rhs.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator>=(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.byte_ == rhs.byte_);

      return lhs.bit_ >= rhs.bit_;
    }

    /**
     * @param[in] lhs.
     * @param[in] rhs.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator<(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.byte_ == rhs.byte_);

      return lhs.bit_ < rhs.bit_;
    }

    /**
     * @param[in] lhs.
     * @param[in] rhs.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] friend constexpr func operator<=(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(lhs.byte_ == rhs.byte_);

      return lhs.bit_ <= rhs.bit_;
    }

    /**
     * @public
     * @param[in] other.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator==(const ConstIterator& other) const noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(byte_ == other.byte_);

      return bit_ == other.bit_;
    }

    /**
     * @public
     * @param[in] other.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator!=(const ConstIterator& other) const noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(byte_ == other.byte_);

      return bit_ != other.bit_;
    }

    /**
     * @public
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator*() const noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

      return byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst]
        & DynamicBitset::bitMask::BIT << (
          static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst
        );
    }

    /**
     * @public
     * @param[in] index.
     * @throws None (no-throw guarantee).
     */
    [[nodiscard]] constexpr func operator[](differenceType index) const noexcept -> bool
    {
      BITS_DYNAMIC_BITSET_ASSERT(byte_ != nullptr);

      return byte_[static_cast<DynamicBitset::sizeType>(index) >> BlockInfo::byteDivConst] 
        & DynamicBitset::bitMask::BIT << (
          static_cast<DynamicBitset::sizeType>(index) & BlockInfo::byteModConst
        );
    }

    /**
     * @public
     * @param[in] other.
     * @throws None (no-throw guarantee).
     */
    constexpr func operator=(const ConstIterator& other) noexcept -> ConstIterator& = default;

   private:
    pointer byte_;
    differenceType bit_;
  };

 private:
  [[nodiscard]] static constexpr func CalculateCapacity(sizeType bits) noexcept -> sizeType
  {
    return (bits >> BlockInfo::byteDivConst) + (bits & BlockInfo::byteModConst ? 1 : 0);
  }

  constexpr func SetBit(sizeType index, bool value) noexcept -> void
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    if (value)
    {
      storage_[index >> BlockInfo::byteDivConst] |= bitMask::BIT << (index & BlockInfo::byteModConst);
    }
    else
    {
      storage_[index >> BlockInfo::byteDivConst] &= ~(bitMask::BIT << (index & BlockInfo::byteModConst));
    }
  }

  [[nodiscard]] constexpr func ResizeFactor() const noexcept -> bool
  {
    return (bits_ >> BlockInfo::byteDivConst) >= blocks_;
  }

  static constexpr func CopyData(
      pointer source,
      pointer destination,
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
      pointer source,
      sizeType n,
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

    std::fill(tempPtr + blocks_, tempPtr + newSize, bitMask::RESET);
    storage_ = tempPtr;
    blocks_ = newSize;
  }

 public:
  /**
   * @public
   * @throws None (no-throw guarantee).
   */
  [[nodiscard]] constexpr func begin() noexcept -> Iterator
  {
    return {storage_, 0};
  }

  /**
   * @public
   * @throws None (no-throw guarantee).
   */
  [[nodiscard]] constexpr func end() noexcept -> Iterator
  {
    return {storage_, static_cast<differenceType>(bits_)};
  }

  /**
   * @public
   * @throws None (no-throw guarantee).
   */
  [[nodiscard]] constexpr func cbegin() const noexcept -> ConstIterator
  {
    return {storage_, 0};
  }

  /**
   * @public
   * @throws None (no-throw guarantee).
   */
  [[nodiscard]] constexpr func cend() const noexcept -> ConstIterator
  {
    return {storage_, static_cast<differenceType>(bits_)};
  }

  /**
   * @public
   * @brief Consturct an empty `DynamicBitset` with default constructed allocator.
   * @details Creates a `DynamicBitset` with zero capacity and no allocated storage.
   * 
   * @throws None (no-throw guarantee).
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits; // Data() == nullptr, Size() == 0, Capacity() == 0
   * @endcode
   */
  constexpr DynamicBitset() noexcept(std::is_nothrow_default_constructible_v<allocatorType>) = default;

  /**
   * @public
   * @brief Construct an empty `DynamicBitset` with a specified allocator.
   * @details Creates a `DynamicBitset` with zero capacity and no allocated storage,
   *          but configures it to use the provided `allocator`.
   * 
   * @param[in] allocator The allocator to use for subsequent memory allocations.
   * 
   * @throws None (no-throw guarantee).
   * 
   * @par Example:
   * @code{.cpp}
   * CustomAllocatorType customAllocator;
   * bits::DynamicBitset<typename CustomAllocatorType::value_type, CustomAllocatorType> bits{customAllocator};
   * bits.Resize(1'000, true); // Uses allocator copy of customAllocatorType instance
   * @endcode
   */
  explicit constexpr DynamicBitset(const allocatorType& allocator) noexcept
      : alloc_{allocator}
  {
    /* Empty */
  }

  /**
   * @public
   * @brief Construct `DynamicBitset` with a specified allocator.
   * @details Creates a `DynamicBitset` with `bits` size, copy-constructed allocator
   *          and first block initialized with `value`.
   * 
   * @param[in] bits The size of the `DynamicBitset` to construct.
   * @param[in] value The value to initialize the first block.
   * @param[in] allocator The allocator to use for subsequent memory allocations.
   * 
   * @throws std::bad_alloc If memory allocation fails.
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, 0xff}; // Size() == 100, bits in {0...7} are set to `true`
   *                                   // [1, 1, 1, 1, 1, 1, 1, 1, 0, ...]
   * bits::DynamicBitset b{10, 0, std::allocator<size_t>{}}; // Size() == 10, bits in {0...9} are set to `false`
   *                                                         // Explicitly passed `std::allocator<size_t>` instance
   * @endcode
   */
  constexpr DynamicBitset(
      sizeType bits,
      blockType value = 0,
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
    std::fill(storage_, storage_ + blocks_, bitMask::RESET);
    *storage_ = value;
  }

  /**
   * @public
   * @brief Copy-constructs a `DynamicBitset` from another instance.
   * @details Constructs a deep copy of `other`, using the copied allocator.
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
  constexpr DynamicBitset(const DynamicBitset& other)
      :  alloc_{std::allocator_traits<allocatorType>::select_on_container_copy_construction(other.alloc_)}
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
   * @details Constructs a deep copy of `other`, using the provided `allocator` for
   *          memory allocation.
   * @param[in] other The `DynamicBitset` to copy resources from.
   * @param[in] allocator The allocator to use for memory management.
   * 
   * @throws std::bad_alloc If memory allocation fails.
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, 0xff};
   * bits::DynamicBitset b{a, anotherAllocator}; // a == b except that may holds different allocator instances
   * @endcode
   */
  constexpr DynamicBitset(
      const DynamicBitset& other, 
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
   * 
   * @param[in] other The `DynamicBitset` to move resources from.
   * 
   * @throws None (no-throw guarantee).
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, 0xff};
   * bits::DynamicBitset b{std::move(a)}; // a.Data() == nullptr, a.Size() == 0, a.Capacity() == 0
   * @endcode
   */
  constexpr DynamicBitset(DynamicBitset&& other) noexcept
    : storage_{other.storage_}
    , bits_{other.bits_}
    , blocks_{other.blocks_}
    , alloc_{std::move(other.alloc_)}
  {
    other.storage_ = nullptr;
    other.bits_ = other.blocks_ = 0;
  }

  /**
   * @public
   * @brief Move-constructs a `DynamicBitset` with a specified allocator.
   * @param[in] other Source `DynamicBitset` to move from.
   * @param[in] allocator Allocator to use for the new `DynamicBitset`.
   * 
   * @throws None if `is_always_equal` is true, otherwise may throw `std::bad_alloc`.
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset a{100, customAllocator};
   * bits::DynamicBitset b{std::move(a), anotherAllocator}; // a is empty if `is_always_equal` is true or `customAllocator == anotherAllocator`,
   *                                                        // unchanged otherwise
   * @endcode
   */
  constexpr DynamicBitset(
      DynamicBitset&& other, 
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
   * @tparam BlockIterator.
   * @param[in] first.
   * @param[in] last.
   * @throws std::bad_alloc If memory allocation fails.
   */
  template<IsValidDynamicBitsetBlockIterator<blockType> BlockIterator>
  constexpr DynamicBitset(BlockIterator first, BlockIterator last) noexcept
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
   * @details Safely deallocates the underlying storage using the associated allocator.
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
   * @brief Returns the current storage capacity in bits.
   * @return The maximum number of bits that can be stored without reallocation.
   * 
   * @throws None (no-throw guarantee).
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * bits.Reserve(100); // Capacity() == 100, Size() == 0
   * bits.PushBack(true); // Capacity() == 100, Size() == 1
   * @endcode
   */
  [[nodiscard]] constexpr func Capacity() const noexcept -> sizeType
  {
    return blocks_ << BlockInfo::byteDivConst;
  }

  /**
   * @public
   * @brief Returns the theoretical maximum number of bits the container could hold.
   * @details This value reflects fundamental limits of:
   *          - The `sizeType` used (e.g., `size_t` maximum).
   *          - The object representation (no memory constraints considered).
   * 
   * @return A constant `sizeType` value equal to `numeric_limits<sizeType>::max()`.
   * @throws None (no-throw guarantee).
   * 
   * @warning This is theoretical upper bound. The true usable capacity may be smaller due to:
   * - Memory availability.
   * - Allocator limitations.
   * - Implementation overhead.
   * 
   * @note Identical to `std::vector::max_size()` in behaviour. Exists primarily for STL API compatability.
   */
  [[nodiscard]] constexpr func MaxSize() const noexcept -> sizeType
  {
    return std::numeric_limits<sizeType>::max();
  }

  /**
   * @public
   * @brief Direct read/write access to the underlying bit storage.
   * @return Pointer to the first block of bit storage.
   * 
   * @throws None (no-throw guarantee).
   * @warning The returned pointer becomes invalid after:
   *  - Any reallocation (`Resize()`, `Reserve()`, etc).
   *  - Destruction of the `DynamicBitset`.
   * 
   * @note Direct comparison with memcmp on pointers may result to non equality due to internal optimizations.
   */
  [[nodiscard]] constexpr func Data() noexcept -> pointer
  {
    return storage_;
  }

  /**
   * @public
   * @brief Direct read-only access to the underlying bit storage.
   * @return Const pointer to the first block of bit storage.
   * 
   * @throws None (no-throw guarantee).
   * @warning The returned pointer becomes invalid after:
   *  - Any reallocation (`Resize()`, `Reserve()`, etc).
   *  - Destruction of the `DynamicBitset`.
   * 
   * @note Direct comparison with memcmp on pointers may result to non equality due to internal optimizations.
   */
  [[nodiscard]] constexpr func Data() const noexcept -> constPointer
  {
    return storage_;
  }

  /**
   * @public
   * @brief Returns a copy of the underlying allocator.
   * @return A copy of the allocator used for bit storage management.
   * 
   * @throws None (no-throw guarantee).
   */
  [[nodiscard]] constexpr func GetAllocator() const noexcept -> allocatorType
  {
    return alloc_;
  }

  /**
   * @public
   * @brief Returns the number of set bits in the `DynamicBitset`.
   * @return The count of set bits (bits with value `true`), uses `std::popcount` and bit extraction.
   * 
   * @throws None (no-throw guarantee).
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{8, 0xff};
   * auto setBits{bits.Count()}; // setBits == 8
   * @endcode
   */
  [[nodiscard]] constexpr func Count() const noexcept -> sizeType
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
      if (*end & bitMask::BIT << currentBit)
      {
        ++bitCount;
      }
    }

    return bitCount;
  }

  /**
   * @public
   * @brief Reduces the capacity to store the minimum required blocks.
   * @details If bit storage contains unused memory blocks the container resizes
   *          to the minimal capacity to hold the current size.
   * 
   * @throws std::bad_alloc if memory allocation fails (original storage remains intact).
   * 
   * @warning If bit storage contains unused blocks:
   *  - Iterators invalidated.
   *  - Pointers to internal storage invalidated.
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * bits.Reserve(4); // Size() == 0, Capacity() == 4 * sizeof(Block) * 8
   * bits.ShrinkToFit(); // Size() == 0, Capacity() == 0
   * bits.Reserve(2); // Size() == 0, Capacity() == 2 * sizeof(Block) * 8
   * bits.PushBack(true); // Size() == 1, Capacity() == 2 * sizeof(Block) * 8
   * bits.ShrinkToFit(); // Size() == 1, Capacity() == sizeof(Block) * 8
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
   * @brief Returns `true` if at least one bit is set, `false` otherwise.
   * @details Scans the bit storage to find first set bit (bit that is set to `true`).
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
  [[nodiscard]] constexpr func Any() const noexcept -> bool
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
      if (*end & bitMask::BIT << currentBit)
      {
        return true;
      }
    }

    return false;
  }

  /**
   * @public
   * @brief Returns `true` if all bits unset, `false` otherwise.
   * @details Scans the bit storage to find at least one set bit (bit that set to `true`).
   *          Returns when block with at least one set bit occurres.
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
  [[nodiscard]] constexpr func None() const noexcept -> bool
  {
    return !Any();
  }

  /**
   * @public
   * @brief Returns `true` if all bits are set, `false` otherwise.
   * @details Scans the bit storage and counts the set bits.
   *          If Size() == Count() returns `true`, `false` otherwise.
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
  [[nodiscard]] constexpr func All() const noexcept -> bool
  {
    return bits_ && bits_ == Count();
  }

  /**
   * @public
   * @brief Checks if the `DynamicBitset` is empty.
   * @details This method determines whether the underlying bit storage is empty.
   *          Equivalent to `Size() == 0`.
   * @see Size()
   * @return `true` if container is empty, `false` otherwise.
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
  [[nodiscard]] constexpr func Empty() const noexcept -> bool
  {
    return !bits_;
  }

  /**
   * @public
   * @brief Releases memory obtained by bit storage.
   * @details Releases memory obtained by bit storage and sets
   *          the container in empty state.
   * @see Empty()
   * 
   * @throws None (no-throw guarantee).
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits{100}; // Size() == 100, Capacity() == depeneds on sizeof(size_t)
   *                                // parameter multiplied by CHAR_BIT (size_t is the default block type)
   * bits.Clear(); // Data() == nullptr, Size() == 0, Capacity() == 0
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
   * @param[in] bits.
   * @param[in] value.
   * @throws std::bad_alloc If memory allocation fails.
   * 
   * @note This operation invalidates all iterators if `bits > Size()` or `bits < Size()`.
   */
  constexpr func Resize(sizeType bits, bool value = false) -> void
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

      FillData(
          storage_ + blocks_,
          newSize - blocks_,
          value ? bitMask::SET : bitMask::RESET
      );
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
   * @param[in] bytes.
   * @throws std::bad_alloc If memory allocation fails.
   * 
   * @note This operation invalidates all iterators.
   */
  constexpr func Reserve(sizeType blocks) -> void
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
   * @param[in] value The bit value `true/false` to append.
   * 
   * @throws std::bad_alloc If memory allocation fails.
   * 
   * @note Iterator invalidation:
   *  - All iterators are invalidated if reallocation occurs.
   *  - Only `end()` iterators are invalidated if no reallocation occurs.
   * 
   * @par Example:
   * @code{.cpp}
   * bits::DynamicBitset bits;
   * bits.PushBack(true); // Sequence: [1]
   * bits.PushBack(false); // Sequence: [1, 0]
   * @endcode
   */
  constexpr func PushBack(bool value) -> void
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
  [[nodiscard]] constexpr func Size() const noexcept -> sizeType
  {
    return bits_;
  }

  /**
   * @public
   * @throws None (no-throw guarantee).
   */
  constexpr func PopBack() noexcept -> void
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr && bits_ > 0);

    --bits_;
  }

  /**
   * @public
   * @param[in] index The zero-based index of the bit to set/unset.
   * @param[in] value The boolean value `true/false`.
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   */
  constexpr func Set(sizeType index, bool value = false) -> DynamicBitset&
  {
    if (index >= bits_)
    {
      throw std::out_of_range{std::format(
          "DynamicBitset::Set(sizeType, bool = false): index {} >= Size ({})",
          index,
          bits_
      )};
    }

    SetBit(index, value);
    return *this;
  }

  /**
   * @public
   * @throws std::out_of_range If `Empty() == true`.
   */
  constexpr func Set() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Set() -> invalid number of bits"};
    }

    FillData(storage_, CalculateCapacity(bits_), bitMask::SET);
    return *this;
  }

  /**
   * @public
   * @param[in] index The zero-based index of the bit to reset.
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   */
  constexpr func Reset(sizeType index) -> DynamicBitset&
  {
    if (index >= bits_)
    {
      throw std::out_of_range{std::format(
          "DynamicBitset::Reset(sizeType): index {} >= Size ({})",
          index,
          bits_
      )};
    }

    storage_[index >> BlockInfo::byteDivConst] &= ~(bitMask::BIT << (index & BlockInfo::byteModConst));
    return *this;
  }

  /**
   * @public
   * @throws std::out_of_range If `Empty() == true`.
   */
  constexpr func Reset() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Reset() -> invalid number of bits"};
    }

    FillData(storage_, CalculateCapacity(bits_), bitMask::RESET);
    return *this;
  }

  /**
   * @public
   * @param[in] index The zero-based index of the bit to flip.
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   */
  constexpr func Flip(sizeType index) -> DynamicBitset&
  {
    BITS_DYNAMIC_BITSET_ASSERT(index < bits_);

    if (index >= bits_)
    {
      throw std::out_of_range{std::format(
          "DynamicBitset::Flip(sizeType): index {} >= Size ({})",
          index,
          bits_
      )};
    }

    storage_[index >> BlockInfo::byteDivConst] ^= bitMask::BIT << (index & BlockInfo::byteModConst);
    return *this;
  }

  /**
   * @public
   * @throws std::out_of_range If container is empty.
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
      *begin ^= bitMask::SET;
    }

    return *this;
  }

  /**
   * @public
   * @brief Swaps the contents of the two bit sequences.
   * @details This member function swaps all internal contents of the
   *          two container objects. Does not modify the container if
   *          this == &other.
   * @param[in] other.
   * @throws None (no-throw guarantee).
   */
  constexpr func Swap(DynamicBitset& other) noexcept -> void
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
   * @brief Provides the access to the bit referenced by given index.
   * @details This operator overload provides the access to the certain bit
   *          using special wrapper class. Non const overload may change the
   *          bit value.
   * @see bits::DynamicBitset::Iterator::BitWrapper.
   * @param[in] index The zero-base index of the bit to access.
   * @return bits::DynamicBitset::Iterator::BitWrapper object to obtain the specified bit.
   * @throws None (no-throw guarantee).
   * @note This operator overload does not check the given index and may lead to UB.
   */
  [[nodiscard]] constexpr func operator[](sizeType index) noexcept -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return {storage_, static_cast<differenceType>(index)};
  }

  /**
   * @public
   * @brief Provides the access to the bit referenced by given index.
   * @details This operator overload provides the access to the certain bit
   *          but only the view of the value stored in it.
   * @param[in] index The zero-based index of the bit to access.
   * @return `true` if bit is set, `false` otherwise.
   * @throws None (no-throw guarantee).
   * @note This operator overload does not check the given index and may lead to UB.
   */
  [[nodiscard]] constexpr func operator[](sizeType index) const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return storage_[index >> BlockInfo::byteDivConst] & bitMask::BIT << (index & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Provides the access to the first bit in bit sequence obtained by the container.
   * @details This member function provides the access to the first bit in
   *          bit sequence using special wrapper class. 
   *          Non const overload may change the bit value.
   * @see bits::DynamicBitset::Iterator::BitWrapper
   * @return `bits::DynamicBitset::Iterator::BitWrapper`.
   * @throws None (no-throw guarantee).
   * @note This member function does not check the container state before invocation.
   *       Invoking Front member function on empty container lead to UB.
   */
  [[nodiscard]] constexpr func Front() noexcept -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return {storage_, 0};
  }

  /**
   * @public
   * @brief Provides the access to the first bit in bit sequence obtained by the container.
   * @details This member function provides the access to the first bit in
   *          bit sequence. Does not modify the container state.
   * @return `true` if bit is set, `false` otherwise.
   * @throws None (no-throw guarantee).
   * @note This member function does not check the container state before invocation.
   *       Invoking Front member function on empty container lead to UB.
   */
  [[nodiscard]] constexpr func Front() const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return *storage_ & bitMask::BIT;
  }

  /**
   * @public
   * @brief Provides the access to the last bit in bit sequence obtained by the container.
   * @details This member function provides the access to the last bit in
   *          bit sequence using special wrapper class.
   *          Non const overload may change the bit value.
   * @see bits::DynamicBitset::Iterator::BitWrapper
   * @return `bits::DynamicBitset::Iterator::BitWrapper`.
   * @throws None (no-throw guarantee).
   * 
   * @warning **Undefined Behaviour** if:
   * - The container is empty `Size() == 0`.
   * 
   * @note For bounds-checking access, use `At()`.
   * @see At()
   */
  [[nodiscard]] constexpr func Back() noexcept -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return {storage_, static_cast<differenceType>(bits_ - 1)};
  }

  /**
   * @public
   * @brief Provides the access to the last bit in sequence without bounds checking.
   * @details This member function provides the access to the last bit in
   *          bit sequence. Does not modify the container state.
   * @return `true` if bit is set, `false` otherwise.
   * @throws None (no-throw guarantee).
   * 
   * @warning **Undefined Behaviour** if:
   * - The container is empty `Size() == 0`.
   * 
   * @note For bounds-checked access, use `At()`.
   * @see At()
   */
  [[nodiscard]] constexpr func Back() const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return storage_[(bits_ - 1) >> BlockInfo::byteDivConst]
      & bitMask::BIT << ((bits_ - 1) & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Accesses the bit at the specified index with bounds checking.
   * @details This function provides read/write access to a bit in a
   *          sequence using `BitWrapper` class. Unlike `operator[]` and
   *          `Test()`, it performs bounds checking and throws an
   *           exception if the index is invalid.
   * @see BitWrapper
   * 
   * @param[in] index The zero-based index of the bit to access.
   * @return `BitWrapper`
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   * 
   * @note For unchecked access, use `operator[]` or `Test()`.
   * @see operator[], Test()
   */
  [[nodiscard]] constexpr func At(sizeType index) -> typename Iterator::BitWrapper
  {
    BITS_DYNAMIC_BITSET_ASSERT(index < bits_);

    if (index >= bits_)
    {
      throw std::out_of_range{std::format(
          "bits::DynamicBitset::At(sizeType): index {} >= Size ({})",
          index,
          bits_
      )};
    }

    return {storage_, static_cast<differenceType>(index)};
  }

  /**
   * @public
   * @brief Accesses the bit at the specified index with bounds checking.
   * @details This function provides read-only access to a bit
   *          in the sequence. Unlike `operator[]` and `Test()`, it performs 
   *          bounds checking and throws an exception if the index is invalid.
   * 
   * @param[in] index The zero-based index of the bit to access.
   * @return `true` if the bit is set, `false` otherwise.
   * @throws std::out_of_range If `index >= Size()` (out-of-bounds access).
   * 
   * @note For unchecked access, use `operator[]` or `Test()`.
   * @see operator[], Test()
   */
  [[nodiscard]] constexpr func At(sizeType index) const -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(index < bits_);

    if (index >= bits_)
    {
      throw std::out_of_range{std::format(
          "bits::DynamicBitset::At(sizeType) const: index {} >= Size ({})",
          index,
          bits_
      )};
    }

    return storage_[index >> BlockInfo::byteDivConst] & bitMask::BIT << (index & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Accesses the bit at the specified index.
   * @details This function provides read-only access to the bit
   *          in the sequence. It does not modify the container and
   *          is equivalent to `operator[] const noexcept`.
   * 
   * @param[in] index The zero-based index of the bit to access.
   * @return `true` if the bit is set, `false` otherwise.
   * @throws None (no-throw guarantee).
   * 
   * @warning **Undefined Behaviour** if:
   * - `index >= Size()` (out-of-bounds access).
   * - The container is empty (`Size() == 0`).
   * 
   * @note For bounds-checked access, use `At()`.
   * @see At()
   */
  [[nodiscard]] constexpr func Test(sizeType index) const noexcept -> bool
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    return storage_[index >> BlockInfo::byteDivConst] & bitMask::BIT << (index & BlockInfo::byteModConst);
  }

  /**
   * @public
   * @brief Copies the contents of another bit sequence.
   * @param[in] other Another bits::DynamicBitset object
   * @return `bits::DynamicBitset&`.
   * @throws std::bad_alloc If reallocation of internal buffer takes place.
   * @note This operator overload may throw an exception when container
   *       resizes to the size of other bit sequence.
   */
  DynamicBitset& operator=(const DynamicBitset& other)
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

    CopyData(other.storage_, storage_, other.blocks_);
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
   * @param[in] other.
   */
  constexpr func operator=(DynamicBitset&& other) noexcept -> DynamicBitset&
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
   * @param[in] other
   */
  constexpr func operator&=(const DynamicBitset& other) -> DynamicBitset&
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    if (bits_ != other.bits_ || !bits_ || !other.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator&= -> invalid storage size"};
    }

    pointer beginRhs{other.storage_};
    pointer end{storage_ + blocks_};

    [[likely]]
    for (pointer beginLhs{storage_}; beginLhs < end; ++beginLhs)
    {
      *beginLhs &= *beginRhs++;
    }

    return *this;
  }

  /**
   * @public
   * @param[in] other
   */
  constexpr func operator|=(const DynamicBitset& other) -> DynamicBitset&
  {
    BITS_DYNAMIC_BITSET_ASSERT(this != &other);

    if (bits_ != other.bits_ || !bits_ || !other.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator|= -> invalid storage size"};
    }

    pointer beginSelf{storage_};
    pointer beginOther{other.storage_};
    pointer end{storage_ + blocks_};

    [[likely]]
    while (beginSelf < end)
    {
      *beginSelf++ |= *beginOther++;
    }

    return *this;
  }

  /**
   * @public
   * @param[in] other
   */
  constexpr func operator^=(const DynamicBitset& other) -> DynamicBitset&
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
   * @return New `DynamicBitset` copy with all reversed bits.
   * @throws std::out_of_range If the container is empty `Empty() == true`.
   */
  [[nodiscard]] constexpr func operator~() const -> DynamicBitset
  {
    BITS_DYNAMIC_BITSET_ASSERT(storage_ != nullptr);

    if (!storage_)
    {
      throw std::out_of_range{"DynamicBitset::operator~ -> invalid storage pointer (nullptr)"};
    }

    auto bits{*this};
    pointer end{bits.storage_ + bits.blocks_};

    [[likely]]
    for (pointer begin{bits.storage_}; begin < end; ++begin)
    {
      *begin ^= bitMask::SET;
    }

    return bits;
  }

  /**
   * @public
   * @param[in] bitOffset.
   * 
   * @throws std::out_of_range If the container is empty `Empty() == true`.
   */
  constexpr func operator>>=(sizeType bitOffset) -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::operator>>= -> invalid storage pointer (nullptr)"};
    }
    else if (bitOffset >= bits_)
    {
      FillData(storage_, CalculateCapacity(bits_), bitMask::RESET);
    }
    else if (bitOffset)
    {
      sizeType totalShifts{bitOffset - 1};

      for (sizeType bit{bits_ - 1}; bit > totalShifts; --bit)
      {
        sizeType bitShift{bit - bitOffset};
        sizeType byte{storage_[bitShift >> BlockInfo::byteDivConst]
          & bitMask::BIT << (bitShift & BlockInfo::byteModConst)
        };
        SetBit(bit, byte);
      }

      FillData(storage_, bitOffset >> BlockInfo::byteDivConst, bitMask::RESET);

      for (sizeType bit{bitOffset - (bitOffset & BlockInfo::byteModConst)}; bit != bitOffset; ++bit)
      {
        SetBit(bit, false);
      }
    }

    return *this;
  }

  /**
   * @public
   * @param[in] bitOffset
   */
  constexpr func operator<<=(sizeType bitOffset) -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::operator<<= -> invalid storage pointer (nullptr)"};
    }
    else if (bitOffset >= bits_)
    {
      FillData(storage_, CalculateCapacity(bits_), bitMask::RESET);
    }
    else if (bitOffset)
    {
      sizeType totalShifts{bits_ - bitOffset};
      for (sizeType bit{}; bit < totalShifts; ++bit)
      {
        sizeType bitShift{bit + bitOffset};
        sizeType state{storage_[bitShift >> BlockInfo::byteDivConst]
          & bitMask::BIT << (bitShift & BlockInfo::byteModConst)
        };
        SetBit(bit, state);
      }
      sizeType byteShift{bitOffset >> BlockInfo::byteDivConst};
      FillData(
          storage_ + (CalculateCapacity(bits_) - byteShift),
          byteShift,
          bitMask::RESET
      );
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
   * @brief Converts the bitset to a human-readable string representation.
   * @return A new `std::string` where each character represents a bit:
   * - '1' for set bits.
   * - '0' for unset bits.
   * - Character order matches bit indices (`strBits[0] == bits[0]).
   * 
   * @throws std::bad_alloc If memory allocation during `std::string` construction fails.
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
  [[nodiscard]] constexpr func ToString() const -> std::string
  {
    std::string strBits(bits_, '\0');

    for (sizeType currentBit{}; currentBit < bits_; ++currentBit)
    {
      strBits[currentBit] =
        (storage_[currentBit >> BlockInfo::byteDivConst] >> (currentBit & BlockInfo::byteModConst)
          & bitMask::BIT) | '0';
    }

    return strBits;
  }

 private:
  pointer storage_{nullptr};
  sizeType bits_{};
  sizeType blocks_{};
  [[no_unique_address]] allocatorType alloc_;
};

END_BITS_NAMESPACE

/**
 * @tparam Block Unsigned integral type for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @returns `true` If `lhs` bits is equal to `rhs`, `false` otherwise.
 * 
 * @throws None (no-throw guarantee).
 */
template<typename Block, typename Alloc>
[[nodiscard]] constexpr func operator==(
    const bits::DynamicBitset<Block, Alloc>& lhs,
    const bits::DynamicBitset<Block, Alloc>& rhs
) noexcept -> bool
{
  BITS_DYNAMIC_BITSET_ASSERT(&lhs != &rhs);

  if (lhs.size() != rhs.size())
  {
    return false;
  }

  auto beginLhs{lhs.Data()};
  auto end{beginLhs + lhs.Capacity()};
  auto beginRhs{rhs.Data()};

  [[likely]]
  while (beginLhs < end)
  {
    if (*beginLhs++ != *beginRhs++)
    {
      return false;
    }
  }
  return true;
}

/**
 * @tparam Block Unsigned integral type for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @returns `true` If `lhs` bits is not equal to `rhs`, `false` otherwise.
 * 
 * @throws None (no-throw guarantee).
 */
template<typename Block, typename Alloc>
[[nodiscard]] constexpr func operator!=(
    const bits::DynamicBitset<Block, Alloc>& lhs,
    const bits::DynamicBitset<Block, Alloc>& rhs
) noexcept -> bool
{
  return !(lhs == rhs);
}

/**
 * @brief Performs a bitwise AND between two `DynamicBitset` objects.
 * @details Returns a new `DynamicBitset` where each bit is the result of:
 *          `lhs[i] & rhs[i]` for all positions.
 *          Delegates to operator&= internally; see its documentation for
 *          AND semantics.
 * @see operator&=
 * 
 * @tparam Block Unsigned integral type for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @return New `DynamicBitset` containing the AND result.
 * 
 * @throws std::bad_alloc If memory allocation fails during copy construction.
 * @note This is non-modifying operation (original `DynamicBitset` objects remains unchanged).
 */
template<typename Block, typename Alloc>
[[nodiscard]] constexpr func operator&(
    const bits::DynamicBitset<Block, Alloc>& lhs,
    const bits::DynamicBitset<Block, Alloc>& rhs
) -> bits::DynamicBitset<Block, Alloc>
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
 * 
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @return New `DynamicBitset` containing the OR result.
 * 
 * @throws std::bad_alloc If memory allocation fails during copy construction.
 * @note This is non-modifying operation (original `DynamicBitset` objects remains unchanged).
 */
template<typename Block, typename Alloc>
[[nodiscard]] constexpr func operator|(
    const bits::DynamicBitset<Block, Alloc>& lhs,
    const bits::DynamicBitset<Block, Alloc>& rhs
) -> bits::DynamicBitset<Block, Alloc>
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
 * 
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBitset` operand.
 * @return New `DynamicBitset` containing the XOR result.
 * 
 * @throws std::bad_alloc If memory allocation fails during copy construction.
 * @note This is non-modifying operation (original `DynamicBitset` objects remains unchanged).
 */
template<typename Block, typename Alloc>
[[nodiscard]] constexpr func operator^(
    const bits::DynamicBitset<Block, Alloc>& lhs,
    const bits::DynamicBitset<Block, Alloc>& rhs
) -> bits::DynamicBitset<Block, Alloc>
{
  BITS_DYNAMIC_BITSET_ASSERT(&lhs != &rhs);

  auto bits{lhs};
  bits ^= rhs;
  return bits;
}

/**
 * @brief Returns a copy of the `DynamicBitset` object left-shifted by `bitOffset` positions.
 * @details This operation applies logical bitwise shift to the sequence.
 *          If `bitOffset >= Size()` all bits will be unset, otherwise pretends
 *          as a builtin operator for fundamental unsigned integral types.
 * 
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] bits Original `DynamicBitset` object to shift.
 * @param[in] bitOffset Number of positions to shift (>= 0).
 * @return New `DynamicBitset` object with shifted bits.
 * 
 * @throws std::bad_alloc If memory allocation fails during copy construction.
 * @note This is non-modifying operation (original `DynamicBitset` object remains unchanged).
 * @note Delegates to `operator<<=` internally; see its documentation for shift semantics.
 * @see operator<<=
 */
template<typename Block, typename Alloc>
[[nodiscard]] constexpr func operator<<(
    const bits::DynamicBitset<Block, Alloc>& bits,
    const size_t bitOffset
) -> bits::DynamicBitset<Block, Alloc>
{
  auto tempBits{bits};
  tempBits <<= bitOffset;
  return tempBits;
}

/**
 * @brief Returns a copy of the `DynamicBitset` object right-shifted by `bitOffset` positions.
 * @details This operation applies logical bitwise shift to the sequence.
 *          If `bitOffset >= Size()` all bits will be unset, otherwise pretends
 *          as a builtin operator for fundamental unsigned integral types.
 * 
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] bits Original `DynamicBitset` object to shift.
 * @param[in] bitOffset Number of positions to shift (>= 0).
 * @return New `DynamicBitset` object with shifted bits.
 * 
 * @throws std::bad_alloc If memory allocation fails during copy construction.
 * @note This is non-modifying operation (original `DynamicBitset` object remains unchanged).
 * @note Delegates to `operator>>=` internally; see its documentation for shift semantics.
 * @see operator>>=
 */
template<typename Block, typename Alloc>
[[nodiscard]] constexpr func operator>>(
    const bits::DynamicBitset<Block, Alloc>& bits,
    const size_t bitOffset
) -> bits::DynamicBitset<Block, Alloc>
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
 * 
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 */
template<typename Block, typename Alloc>
struct formatter<bits::DynamicBitset<Block, Alloc>>
    : public formatter<string>
{
  [[nodiscard]] func format(
      const bits::DynamicBitset<Block, Alloc> bits,
      format_context& ctx
  ) const -> decltype(formatter<string>::format(bits.ToString(), ctx))
  {
    return formatter<string>::format(bits.ToString(), ctx);
  }
};

/**
 * @brief Exchanges the contents of two `DynamicBitset` objects.
 * @tparam Block Unsigned integral type used for bit storage (e.g., `uint8_t`).
 * @tparam Alloc Allocator type meeting Cpp17Allocator requirements.
 * 
 * @param[in] lhs First `DynamicBitset` operand.
 * @param[in] rhs Second `DynamicBItset` operand.
 * 
 * @note All elements and allocators (if propogate_on_container_swap is not equal to std::false_type)
 *       are exchanged between `lhs` and `rhs`.
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
    bits::DynamicBitset<Block, Alloc>& lhs,
    bits::DynamicBitset<Block, Alloc>& rhs
) noexcept -> void
{
  lhs.Swap(rhs);
}

} /* End namespace std */

END_EXPORT_MODULE_SECTION