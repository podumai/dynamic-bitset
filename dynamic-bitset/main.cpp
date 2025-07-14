#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <boost/type_index.hpp>
#include <dynamic_bitset/dynamic_bitset.hpp>


auto main() -> int
{
  bits::DynamicBitset bits(16UL, 0xffUL);
  std::cout << bits.ToString() << std::endl;
  auto start{bits.begin()};
  auto end{bits.end() - 1};
  while (start < end)
  {
    bool bitValue{static_cast<bool>(*start)};
    *start = *end;
    *end = bitValue;
    ++start;
    --end;
  }
  std::cout << bits.ToString() << std::endl;
  return 0;
}