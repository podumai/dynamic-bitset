#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <boost/type_index.hpp>
#include <dynamic_bitset/dynamic_bitset.hpp>


auto main() -> int
{
  std::vector<unsigned long> longs(2, 0);
  bits::DynamicBitset<unsigned long> bits(longs.cbegin(), longs.cend());
  bits::DynamicBitset<unsigned long> anotherBits(bits.begin(), bits.end());
  std::cout << anotherBits.ToString() << ' ' << anotherBits.Capacity() << std::endl;
  return 0;
}