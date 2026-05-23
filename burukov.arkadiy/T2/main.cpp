#include "datastruct.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

int main()
{
  using data_t = burukov::DataStruct;

  std::vector< data_t > data;

  using input_iterator_t = std::istream_iterator< data_t >;

  while (std::cin)
  {
    std::copy(
      input_iterator_t{ std::cin },
      input_iterator_t{},
      std::back_inserter(data));

    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();

      std::cin.ignore(
        std::numeric_limits< std::streamsize >::max(),
        '\n');
    }
  }

  std::sort(data.begin(), data.end());

  using output_iterator_t = std::ostream_iterator< data_t >;

  std::copy(
    data.begin(),
    data.end(),
    output_iterator_t{ std::cout, "\n" });

  return 0;
}
