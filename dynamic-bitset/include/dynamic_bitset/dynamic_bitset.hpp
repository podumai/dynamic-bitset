#pragma once

#define func auto

#include <iterator> /* iterator_traits, Iterator concepts */
#include <memory> /* std::allocator<T> */
#include <bitset> /* std::bitset<N> */
#include <stdexcept> /* std::out_of_range, std::length_error, std::invalid_argument */
#include <concepts> /* std::unsigned_integral */
#include <cstdint> /* std::size_t, std::ptrdiff_t */

namespace __bits_details
{

template<typename Block>
concept IsValidDynamicBitsetBlockType = std::unsigned_integral<Block> && !std::is_same_v<Block, bool>;

/** Need to continue requirement for valid alloc and it's type */
template<typename Alloc>
concept IsValidDynamicBitsetAllocType = std::unsigned_integral<typename std::allocator_traits<Alloc>::value_type>
                                       && !std::is_same_v<typename std::allocator_traits<Alloc>::value_type, bool>;

} /* End namespace __bits_details */

namespace bits
{

template<
    __bits_details::IsValidDynamicBitsetBlockType Block = size_t,
    __bits_details::IsValidDynamicBitsetAllocType Alloc = std::allocator<Block>
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
    static constexpr sizeType bitsCount{sizeof(blockType) << 3};
    static constexpr sizeType byteDivConst{bitsCount < 32 ? sizeof(blockType) + 2 : bitsCount == 32 ? 5 : 6};
    static constexpr sizeType byteModConst{bitsCount - 1};
  };

 public:
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
    class BitWrapper final
    {
     private:
      friend Iterator;

     public:
      constexpr BitWrapper() noexcept = default;

      constexpr BitWrapper(
          pointer ptr,
          differenceType bitPosition
      ) noexcept
          : byte_{ptr},
            bit_{bitPosition}
      {
        /* Empty */
      }

      constexpr BitWrapper(const BitWrapper&) noexcept = default;
      constexpr ~BitWrapper() = default;

      constexpr func operator=(bool value) noexcept -> BitWrapper&
      {
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

      constexpr func operator=(const BitWrapper& other) noexcept -> BitWrapper&
      {
        if (this == &other)
        {
          return *this;
        }
        if (other.GetBit())
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

      constexpr func operator|=(bool value) noexcept -> BitWrapper&
      {
        if (value)
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] |=
            DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
        }
        return *this;
      }

      constexpr func operator&=(bool value) noexcept -> BitWrapper&
      {
        if (!value)
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &=
            DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
        }
        return *this;
      }

      constexpr func operator^=(bool value) noexcept -> BitWrapper&
      {
        if (value)
        {
          byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] ^=
            DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
        }
        return *this;
      }

      [[nodiscard]] constexpr func GetBit() const noexcept -> bool
      {
        return byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst] &
          DynamicBitset::bitMask::BIT << (static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst);
      }

      [[nodiscard]] constexpr func operator==(const BitWrapper& other) const noexcept -> bool
      {
        if (!byte_ || !other.byte_)
        {
          return false;
        }
        return GetBit() == other.GetBit();
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

     private:
      pointer byte_;
      differenceType bit_;
    };

   public:
    constexpr Iterator() noexcept = default;

   private:
    constexpr Iterator(
        pointer ptr, 
        differenceType bitPosition
    ) noexcept
        : wrapper_{ptr, bitPosition}
    {
      /* Empty */
    }

   public:
    constexpr Iterator(const Iterator&) noexcept = default;
    constexpr ~Iterator() = default;

    constexpr func swap(Iterator& other) noexcept -> void
    {
      wrapper_.swap(other.wrapper_);
    }

    constexpr func operator++() noexcept -> Iterator&
    {
      ++wrapper_.bit_;
      return *this;
    }

    constexpr func operator++(int) noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_++};
    }

    constexpr func operator+=(differenceType index) noexcept -> Iterator&
    {
      wrapper_.bit_ += index;
      return *this;
    }

    [[nodiscard]] constexpr func operator+(differenceType index) const noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_ + index};
    }

    [[nodiscard]] friend constexpr func operator+(differenceType index, const Iterator& iter) noexcept -> Iterator
    {
      return {iter.wrapper_.byte_, iter.wrapper_.bit_ + index};
    }

    constexpr func operator--() noexcept -> Iterator&
    {
      --wrapper_.bit_;
      return *this;
    }

    constexpr func operator--(int) noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_--};
    }

    constexpr func operator-=(differenceType index) noexcept -> Iterator&
    {
      wrapper_.bit_ -= index;
      return *this;
    }

    [[nodiscard]] constexpr func operator-(differenceType index) const noexcept -> Iterator
    {
      return {wrapper_.byte_, wrapper_.bit_ - index};
    }

    [[nodiscard]] constexpr func operator-(const Iterator& other) const noexcept -> differenceType
    {
      return wrapper_.bit_ - other.wrapper_.bit_;
    }

    [[nodiscard]] friend constexpr func operator<(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      return lhs.wrapper_.bit_ < rhs.wrapper_.bit_;
    }

    [[nodiscard]] friend constexpr func operator<=(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      return lhs.wrapper_.bit_ <= rhs.wrapper_.bit_;
    }

    [[nodiscard]] friend constexpr func operator>(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      return lhs.wrapper_.bit_ > rhs.wrapper_.bit_;
    }

    [[nodiscard]] friend constexpr func operator>=(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      return lhs.wrapper_.bit_ >= rhs.wrapper_.bit_;
    }

    [[nodiscard]] friend constexpr func operator==(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      return lhs.wrapper_.bit_ == rhs.wrapper_.bit_;
    }

    [[nodiscard]] friend constexpr func operator!=(const Iterator& lhs, const Iterator& rhs) noexcept -> bool
    {
      return lhs.wrapper_.bit_ != rhs.wrapper_.bit_;
    }

    [[nodiscard]] constexpr func operator*() noexcept -> BitWrapper&
    {
      return wrapper_;
    }

    [[nodiscard]] constexpr func operator[](differenceType index) noexcept -> BitWrapper
    {
      return {wrapper_.byte_, wrapper_.bit_ + index};
    }

    constexpr Iterator& operator=(const Iterator&) noexcept = default;

   private:
    BitWrapper wrapper_;
  };

  class ConstIterator
  {
   public:
    using difference_type = typename DynamicBitset::difference_type;
    using differenceType = typename DynamicBitset::differenceType;
    using value_type = typename DynamicBitset::value_type;
    using valueType = typename DynamicBitset::valueType;
    using pointer = typename DynamicBitset::pointer;
    using iterator_category = typename std::contiguous_iterator_tag;
    using iteratorCategory = typename std::contiguous_iterator_tag;

   public:
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

    constexpr ConstIterator(const ConstIterator&) noexcept = default;
    constexpr ~ConstIterator() = default;

    constexpr func operator++() noexcept -> ConstIterator&
    {
      ++bit_;
      return *this;
    }

    [[nodiscard]] constexpr func operator++(int) noexcept -> ConstIterator
    {
      return {byte_, bit_++};
    }

    constexpr func operator+=(differenceType index) noexcept -> ConstIterator&
    {
      bit_ += index;
      return *this;
    }

    [[nodiscard]] constexpr func operator+(differenceType index) noexcept -> ConstIterator
    {
      return {byte_, bit_ + index};
    }

    [[nodiscard]] friend constexpr func operator+(
        differenceType index,
        const ConstIterator& iter
    ) noexcept -> ConstIterator
    {
      return {iter.byte_, iter.bit_ + index};
    }

    constexpr func operator--() noexcept -> ConstIterator&
    {
      --bit_;
      return *this;
    }

    [[nodiscard]] constexpr func operator--(int) noexcept -> ConstIterator
    {
      return {byte_, bit_--};
    }

    constexpr func operator-=(differenceType index) noexcept -> ConstIterator&
    {
      bit_ -= index;
      return *this;
    }

    [[nodiscard]] constexpr func operator-(differenceType index) noexcept -> ConstIterator
    {
      return {byte_, bit_ - index};
    }

    [[nodiscard]] friend constexpr func operator-(
        differenceType index,
        const ConstIterator& other
    ) noexcept -> ConstIterator
    {
      return {other.byte_, other.bit_ - index};
    }

    [[nodiscard]] constexpr func operator-(const ConstIterator& other) noexcept -> differenceType
    {
      return bit_ - other.bit_;
    }

    [[nodiscard]] friend constexpr func operator>(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      return lhs.bit_ > rhs.bit_;
    }

    [[nodiscard]] friend constexpr func operator>=(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      return lhs.bit_ >= rhs.bit_;
    }

    [[nodiscard]] friend constexpr func operator<(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      return lhs.bit_ < rhs.bit_;
    }

    [[nodiscard]] friend constexpr func operator<=(
        const ConstIterator& lhs,
        const ConstIterator& rhs
    ) noexcept -> bool
    {
      return lhs.bit_ <= rhs.bit_;
    }

    [[nodiscard]] constexpr func operator==(const ConstIterator& other) const noexcept -> bool
    {
      return bit_ == other.bit_;
    }

    [[nodiscard]] constexpr func operator!=(const ConstIterator& other) const noexcept -> bool
    {
      return bit_ != other.bit_;
    }

    [[nodiscard]] constexpr func operator*() const noexcept -> bool
    {
      return byte_[static_cast<DynamicBitset::sizeType>(bit_) >> BlockInfo::byteDivConst]
        & DynamicBitset::bitMask::BIT << (
          static_cast<DynamicBitset::sizeType>(bit_) & BlockInfo::byteModConst
        );
    }

    [[nodiscard]] constexpr func operator[](differenceType index) noexcept -> bool
    {
      return byte_[static_cast<DynamicBitset::sizeType>(index) >> BlockInfo::byteDivConst] 
        & DynamicBitset::bitMask::BIT << (
          static_cast<DynamicBitset::sizeType>(index) & BlockInfo::byteModConst
        );
    }

    constexpr func operator=(const ConstIterator&) noexcept -> ConstIterator& = default;

   private:
    pointer byte_;
    differenceType bit_;
  };

 private:
  [[nodiscard]] static constexpr func CalculateCapacity(sizeType bits) const noexcept -> sizeType
  {
    return (bits >> BlockInfo::byteDivConst) + (bits & BlockInfo::byteModConst ? 1 : 0);
  }

  constexpr func SetBit(sizeType index, bool value) noexcept -> void
  {
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
    return (bits_ >> BlockInfo::byteDivConst) == bytes_;
  }

  static constexpr func CopyData(
      pointer source,
      pointer destination,
      sizeType n
  ) noexcept -> void
  {
    pointer End{source + n};

    [[likely]]
    while (source < End)
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
    pointer End{source + n};

    [[likely]]
    while (source < End)
    {
      *source++ = value;
    }
  }

  constexpr func GrowInit() -> void
  {
    const sizeType newSize{(bytes_ << 1UL) + 2UL};
    pointer tempPtr{std::allocator_traits<allocatorType>::allocate(alloc_, newSize)};

    if (storage_)
    {
      std::copy(storage_, storage_ + bytes_, tempPtr);
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
    }

    /**
     * This step required for constexpr use.
     */
    std::fill(tempPtr + bytes_, tempPtr + newSize, bitMask::RESET);
    bytes_ = newSize;
    storage_ = tempPtr;
  }

 public:
  [[nodiscard]] constexpr func begin() noexcept -> Iterator
  {
    return {storage_, 0};
  }

  [[nodiscard]] constexpr func end() noexcept -> Iterator
  {
    return {storage_, static_cast<differenceType>(bits_)};
  }

  [[nodiscard]] constexpr func cbegin() const noexcept -> ConstIterator
  {
    return {storage_, 0};
  }

  [[nodiscard]] constexpr func cend() const noexcept -> ConstIterator
  {
    return {storage_, bits_};
  }

  constexpr DynamicBitset() noexcept(noexcept(allocator_type{})) = default;

  explicit constexpr DynamicBitset(
      [[maybe_unused]] const allocator_type& allocator
  ) noexcept
      : alloc_{allocator}
  {
    /* Empty */
  }

  constexpr DynamicBitset(
      sizeType bits,
      blockType value = 0,
      [[maybe_unused]] const allocatorType& allocator = allocatorType{}
  )
      : bits_{bits}
      , bytes_{CalculateCapacity(bits)}
      , alloc_{allocator}
  {
    if (!bits)
    {
      return;
    }

    storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, bytes_);
    FillData(storage_, bytes_, bitMask::RESET);
//    std::fill(storage_, storage_ + bytes_, bitMask::RESET);
    if (value)
    {
      *storage_ = value;
    }
  }

  constexpr DynamicBitset(const DynamicBitset& other)
      : bits_{other.bits_}
      , bytes_{other.bytes_}
  {
    if (bytes_)
    {
      storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, bytes_);
      CopyData(other.storage_, storage_, bytes_);
    }
  }

  constexpr DynamicBitset(
      const DynamicBitset& other, 
      [[maybe_unused]] const allocator_type& allocator
  )
      : bits_{other.bits_}
      , bytes_{other.bytes_}
      , alloc_{allocator}
  {
    if (bytes_)
    {
      storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, bytes_);
      CopyData(other.storage_, storage_, bytes_);
    }
  }

  constexpr DynamicBitset(DynamicBitset&& other) noexcept(noexcept(allocator_type{}))
    : storage_{other.storage_}
    , bits_{other.bits_}
    , bytes_{other.bytes_}
  {
    other.storage_ = nullptr;
    other.bits_ = other.bytes_ = 0;
  }

  constexpr DynamicBitset(
      DynamicBitset&& other, 
      [[maybe_unused]] const allocator_type& allocator
  ) noexcept
      : storage_{other.storage_}
      , bits_{other.bits_}
      , bytes_{other.bytes_}
      , alloc_{allocator}
  {
    other.storage_ = nullptr;
    other.bits_ = other.bytes_ = 0;
  }

  template<std::input_iterator BlockIterator>
  constexpr DynamicBitset(BlockIterator first, BlockIterator last) noexcept
  {
    const sizeType size{std::distance(first, last)};
    if (!size)
    {
      return;
    }
    storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, size);
    bytes_ = size;
    bits_ = size << BlockInfo::byteDivConst;
    for (pointer begin{storage_}; first != last; ++first, ++begin)
    {
      *begin = *first;
    }
  }

  constexpr ~DynamicBitset()
  {
    if (storage_)
    {
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
      storage_ = nullptr;
    }
  }

  /// TODO: shift
  [[nodiscard]] constexpr func Capacity() const noexcept -> sizeType
  {
    return bytes_ << BlockInfo::byteDivConst;
  }

  [[nodiscard]] constexpr func MaxSize() const noexcept -> sizeType
  {
    return std::numeric_limits<sizeType>::max();
  }

  [[nodiscard]] constexpr func Data() noexcept -> pointer
  {
    return storage_;
  }

  [[nodiscard]] constexpr func Data() const noexcept -> constPointer
  {
    return storage_;
  }

  [[nodiscard]] constexpr func GetAllocator() const noexcept(noexcept(alloc_(allocatorType{}))) -> allocatorType
  {
    return alloc_;
  }

  [[nodiscard]] constexpr func Count() const noexcept -> sizeType
  {
    if (!storage_)
    {
      return 0;
    }

    pointer end{storage_ + CalculateCapacity(bits_) - 1};
    sizeType bitCount{};

    for (pointer begin{storage_}; begin < end; ++begin)
    {
      bitCount += std::bitset<BlockInfo::bitsCount>(*begin).count(); /* Works for now */
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

  constexpr func ShrinkToFit() -> void
  {
    if (!bits_)
    {
      Clear();
      return;
    }

    const sizeType currentBytes{CalculateCapacity(bits_)};

    if (currentBytes < bytes_)
    {
      pointer tempPtr{std::allocator_traits<allocatorType>::allocate(alloc_, currentBytes)};
      CopyData(storage_, tempPtr, bytes_);
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
      storage_ = tempPtr;
      bytes_ = currentBytes;
    }
  }

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

  [[nodiscard]] constexpr func None() const noexcept -> bool
  {
    if (!storage_ || !bits_)
    {
      return true;
    }
    pointer end{storage_ + CalculateCapacity(bits_) - 1};

    for (pointer begin{storage_}; begin < end; ++begin)
    {
      if (*begin)
      {
        return false;
      }
    }

    sizeType remainingBits{bits_ & BlockInfo::byteModConst};
    if (!remainingBits)
    {
      remainingBits = BlockInfo::bitsCount;
    }

    for (sizeType currentBit{}; currentBit < remainingBits; ++currentBit)
    {
      if (*end & DynamicBitset::bitMask::BIT << currentBit)
      {
        return false;
      }
    }

    return true;
  }

  [[nodiscard]] constexpr func Empty() const noexcept -> bool
  {
    return !bits_;
  }

  // Modifiers
  constexpr void Clear() noexcept
  {
    if (!storage_)
    {
      return;
    }
    std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
    storage_ = nullptr;
    bits_ = bytes_ = 0;
  }

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

    if (bytes_ < newSize)
    {
      pointer temp_ptr{std::allocator_traits<allocatorType>::allocate(alloc_, newSize)};

      if (storage_)
      {
        CopyData(storage_, temp_ptr, bytes_);
        std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
      }

      storage_ = temp_ptr;

      FillData(
          storage_ + bytes_,
          newSize - bytes_,
          value ? bitMask::SET : bitMask::RESET
      );
      bytes_ = newSize;
      bits_ = bits;

      return;
    }

    while (bits_ < bits)
    {
      SetBit(bits_++, value);
    }
  }

  constexpr func Reserve(sizeType bytes) -> void
  {
    if (!bytes)
    {
      return;
    }

    const sizeType newSize{bytes_ + bytes};
    pointer tempPtr{std::allocator_traits<allocatorType>::allocate(alloc_, newSize)};

    if (storage_)
    {
      CopyData(storage_, tempPtr, bytes_);
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
    }

    storage_ = tempPtr;
    bytes_ = newSize;
  }

  constexpr func PushBack(bool value) -> void
  {
    if (ResizeFactor())
    {
      GrowInit();
    }

    SetBit(bits_++, value);
  }

  [[nodiscard]] constexpr func Size() const noexcept -> sizeType
  {
    return bits_;
  }

  constexpr func PopBack() noexcept -> void
  {
    --bits_;
  }

  constexpr func Set(sizeType index, bool value = false) -> DynamicBitset&
  {
    if (index >= bits_)
    {
      throw std::out_of_range{"DynamicBitset::Set(sizeType, bool = false) -> index is out of range"};
    }

    SetBit(index, value);
    return *this;
  }

  constexpr func Set() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Set() -> invalid number of bits"};
    }

    FillData(storage_, CalculateCapacity(bits_), bitMask::SET);
    return *this;
  }

  constexpr func Reset(sizeType index) -> DynamicBitset&
  {
    if (index >= bits_)
    {
      throw std::out_of_range{"DynamicBitset::Reset(sizeType) -> index is out of range"};
    }

    storage_[index >> BlockInfo::byteDivConst] &= ~(bitMask::BIT << (index & BlockInfo::byteModConst));
    return *this;
  }

  constexpr func Reset() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Reset() -> invalid number of bits"};
    }

    FillData(storage_, CalculateCapacity(bits_), bitMask::RESET);
    return *this;
  }

  constexpr func Flip(sizeType index) -> DynamicBitset&
  {
    if (index >= bits_)
    {
      throw std::out_of_range{"DynamicBitset::Flip(sizeType) -> index is out of range"};
    }

    storage_[index >> BlockInfo::byteDivConst] ^= bitMask::BIT << (index & BlockInfo::byteModConst);
    return *this;
  }

  constexpr func Flip() -> DynamicBitset&
  {
    if (!bits_)
    {
      throw std::out_of_range{"DynamicBitset::Flip() -> invalid number of bits"};
    }

    pointer End{storage_ + CalculateCapacity(bits_)};

    [[likely]]
    for (pointer Begin{storage_}; Begin < End; ++Begin)
    {
      *Begin ^= bitMask::SET;
    }

    return *this;
  }

  constexpr func Swap(DynamicBitset& other) noexcept -> void
  {
    if (this == &other)
    {
      return;
    }
    std::swap(storage_, other.storage_);
    std::swap(bits_, other.bits_);
    std::swap(bytes_, other.bytes_);
  }

  // Element access
  [[nodiscard]] constexpr func operator[](sizeType index) noexcept -> decltype(*Iterator{})
  {
    return typename Iterator::BitWrapper{storage_, static_cast<differenceType>(index)};
  }

  [[nodiscard]] constexpr func operator[](sizeType index) const noexcept -> bool
  {
    return storage_[index >> BlockInfo::byteDivConst] & bitMask::BIT << (index & BlockInfo::byteModConst);
  }

  [[nodiscard]] constexpr func Front() noexcept -> std::remove_reference_t<decltype(*Iterator{})>
  {
    return {storage_, 0};
  }

  [[nodiscard]] constexpr func Front() const noexcept -> bool
  {
    return *storage_ & bitMask::BIT;
  }

  [[nodiscard]] constexpr func Back() noexcept -> decltype(*Iterator{})
  {
    return {storage_, static_cast<differenceType>(bits_ - 1UL)};
  }

  [[nodiscard]] constexpr func Back() const noexcept -> bool
  {
    return storage_[(bits_ - 1) >> BlockInfo::byteDivConst]
      & bitMask::BIT << ((bits_ - 1) & BlockInfo::byteModConst);
  }

  [[nodiscard]] constexpr func At(sizeType index) -> decltype(*Iterator{})
  {
    if (index >= bits_)
    {
      throw std::out_of_range{"DynamicBitset::At(size_type) -> index is out of range"};
    }

    return {storage_, static_cast<differenceType>(index)};
  }

  [[nodiscard]] constexpr func At(sizeType index) const -> bool
  {
    if (index >= bits_)
    {
      throw std::out_of_range{"DynamicBitset::At(size_type) -> index is out of range"};
    }

    return storage_[index >> BlockInfo::byteDivConst] & bitMask::BIT << (index & BlockInfo::byteModConst);
  }

  [[nodiscard]] constexpr func Test(sizeType index) const noexcept -> bool
  {
    return storage_[index >> BlockInfo::byteDivConst] & bitMask::BIT << (index & BlockInfo::byteModConst);
  }

  // Operations
  DynamicBitset& operator=(const DynamicBitset& other)
  {
    if (this == &other)
    {
      return *this;
    }
    if (bytes_ != other.bytes_)
    {
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
      storage_ = std::allocator_traits<allocatorType>::allocate(alloc_, other.bytes_);
    }

    CopyData(other.storage_, storage_, other.bytes_);
    bits_ = other.bits_;
    bytes_ = other.bytes_;

    return *this;
  }

  constexpr func operator=(DynamicBitset&& other) noexcept -> DynamicBitset&
  {
    if (this == &other)
    {
      return *this;
    }
    if (storage_ != nullptr)
    {
      std::allocator_traits<allocatorType>::deallocate(alloc_, storage_, bytes_);
      storage_ = nullptr;
      bits_ = bytes_ = 0;
    }

    std::swap(storage_, other.storage_);
    std::swap(bits_, other.bits_);
    std::swap(bytes_, other.bytes_);

    return *this;
  }

  constexpr func operator&=(const DynamicBitset& rhs) -> DynamicBitset&
  {
    if (bits_ != rhs.bits_ || !bits_ || !rhs.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator&= -> invalid storage size"};
    }

    pointer beginLhs{storage_};
    pointer beginRhs{rhs.storage_};
    pointer End{storage_ + bytes_};

    [[likely]]
    while (beginLhs < End)
    {
      *beginLhs++ &= *beginRhs++;
    }

    return *this;
  }

  constexpr func operator|=(const DynamicBitset& rhs) -> DynamicBitset&
  {
    if (bits_ != rhs.bits_ || !bits_ || !rhs.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator|= -> invalid storage size"};
    }

    pointer beginLhs{storage_};
    pointer beginRhs{rhs.storage_};
    pointer End{storage_ + bytes_};

    [[likely]]
    while (beginLhs < End)
    {
      *beginLhs++ |= *beginRhs++;
    }

    return *this;
  }

  constexpr func operator^=(const DynamicBitset& rhs) -> DynamicBitset&
  {
    if (bits_ != rhs.bits_ || !bits_ || !rhs.bits_)
    {
      throw std::invalid_argument{"DynamicBitset::operator^= -> invalid storage size"};
    }

    pointer beginLhs{storage_};
    pointer beginRhs{rhs.storage_};
    pointer End{storage_ + bytes_};

    [[likely]]
    while (beginLhs < End)
    {
      *beginLhs++ ^= *beginRhs++;
    }

    return *this;
  }

  [[nodiscard]] constexpr func operator~() const -> DynamicBitset
  {
    if (!storage_)
    {
      throw std::out_of_range{"DynamicBitset::operator~ -> invalid storage pointer (nullptr)"};
    }

    auto bits{*this};
    pointer End{bits.storage_ + bits.bytes_};

    [[likely]]
    for (pointer Begin{bits.storage_}; Begin < End; ++Begin)
    {
      *Begin ^= bitMask::SET;
    }

    return bits;
  }

  constexpr func operator>>=(size_type bitOffset) -> DynamicBitset&
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
  sizeType bytes_{};
  [[no_unique_address]] allocatorType alloc_;
};

} /* End namespace bits */

template<typename Alloc1, typename Alloc2>
[[nodiscard]] constexpr func operator==(
    const bits::DynamicBitset<Alloc1>& lhs,
    const bits::DynamicBitset<Alloc2>& rhs
) noexcept -> bool
{
  if (lhs.size() != rhs.size())
  {
    return false;
  }

  auto beginLhs{lhs.Data()};
  auto End{beginLhs + lhs.Capacity()};
  auto beginRhs{rhs.Data()};

  [[likely]]
  while (beginLhs < End)
  {
    if (*beginLhs++ != *beginRhs++)
    {
      return false;
    }
  }
  return true;
}

template<typename Alloc1, typename Alloc2>
[[nodiscard]] constexpr func operator!=(
    const bits::DynamicBitset<Alloc1>& lhs,
    const bits::DynamicBitset<Alloc2>& rhs
) noexcept -> bool
{
  return !(lhs == rhs);
}

template<typename Alloc>
[[nodiscard]] constexpr func operator&(
    const bits::DynamicBitset<Alloc>& lhs,
    const bits::DynamicBitset<Alloc>& rhs
) -> bits::DynamicBitset<Alloc>
{
  auto bits{lhs};
  bits &= rhs;
  return bits;
}

template<class Alloc>
[[nodiscard]] constexpr func operator|(
    const bits::DynamicBitset<Alloc>& lhs,
    const bits::DynamicBitset<Alloc>& rhs
) -> bits::DynamicBitset<Alloc>
{
  auto bits{lhs};
  bits |= rhs;
  return bits;
}

template<class Alloc>
[[nodiscard]] constexpr func operator^(
    const bits::DynamicBitset<Alloc>& lhs,
    const bits::DynamicBitset<Alloc>& rhs
) -> bits::DynamicBitset<Alloc>
{
  auto bits{lhs};
  bits ^= rhs;
  return bits;
}

template<class Alloc>
[[nodiscard]] constexpr func operator<<(
    const bits::DynamicBitset<Alloc>& bits,
    const size_t bitOffset
) -> bits::DynamicBitset<Alloc>
{
  auto tempBits{bits};
  tempBits <<= bitOffset;
  return tempBits;
}

template<class Alloc>
[[nodiscard]] constexpr func operator>>(
    const bits::DynamicBitset<Alloc>& bits,
    const size_t bitOffset
) -> bits::DynamicBitset<Alloc>
{
  auto tempBits{bits};
  tempBits >>= bitOffset;
  return tempBits;
}

namespace std
{

template<class Alloc>
struct formatter<bits::DynamicBitset<Alloc>>
    : formatter<string>
{
  [[nodiscard]] func format(
      const bits::DynamicBitset<Alloc> bits,
      format_context& ctx
  ) const -> decltype(formatter<string>::format(bits.ToString(), ctx))
  {
    return formatter<string>::format(bits.ToString(), ctx);
  }
};

template<class Alloc>
constexpr func swap(
    bits::DynamicBitset<Alloc>& lhs,
    bits::DynamicBitset<Alloc>& rhs
) noexcept -> void
{
  lhs.swap(rhs);
}

} /* End namespace std */

#undef func