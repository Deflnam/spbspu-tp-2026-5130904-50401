#include "datastruct.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

int main()
{
  using T = burukov::DataStruct;
  std::vector<T> data;

  using iit_t = std::istream_iterator<T>;
  while (std::cin)
  {
    std::copy(
        iit_t{ std::cin },
        iit_t{},
        std::back_inserter(data));
        
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(),'\n');
    }
  }

  std::sort(data.begin(), data.end());

  using oit_t = std::ostream_iterator<T>;
  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});

  return 0;
}
