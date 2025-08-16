module;

#include <concepts>    /* std::unsigned_integral<T> */
#include <memory>      /* std::allocator_traits<Allocator> */
#include <type_traits> /* std::is_same_v<T, U>, std::is_convertible_v<T, U> */

export module dynamic_bitset : concepts;

#define BEGIN_EXPORT_PARTITION_CONCEPTS export {
#define END_EXPORT_PARTITION_CONCEPTS }

// BEGIN_EXPORT_PARTITION_CONCEPTS

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
template<typename Allocator>
concept IsValidDynamicBitsetAllocatorType =
  std::unsigned_integral<typename std::allocator_traits<Allocator>::value_type> &&
  !std::is_same_v<typename std::allocator_traits<Allocator>::value_type, bool> &&
  !(sizeof(typename std::allocator_traits<Allocator>::value_type) &
    (sizeof(typename std::allocator_traits<Allocator>::value_type) - 1));

/**
 * @brief Validates the passed Block iterator type.
 * @details Validates the passed Block iterator type to be convertible to block type.
 * @concept IsValidDynamicBitsetBlockIterator
 */
template<typename BlockIterator, typename TargetBlock>
concept IsValidDynamicBitsetBlockIterator = std::is_convertible_v<decltype(*BlockIterator{}), TargetBlock>;

// END_EXPORT_PARTITION_CONCEPTS