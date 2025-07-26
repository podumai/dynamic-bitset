#include <algorithm>
#include <boost/type_index.hpp>
#include <chrono>
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <iostream>
#include <vector>

auto main() -> int
{
  std::vector<unsigned long> longs(2, 0);
  bits::DynamicBitset<unsigned long> bits(longs.cbegin(), longs.cend());
  bits::DynamicBitset<unsigned long> anotherBits(bits.begin(), bits.end());
  std::cout << anotherBits.to_string() << ' ' << anotherBits.capacity() << std::endl;
  return 0;
}