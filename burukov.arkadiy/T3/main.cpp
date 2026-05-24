#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "cli.hpp"
#include "polygon.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file)
  {
    std::cerr << "Cannot open file: " << argv[1] << '\n';
    return 1;
  }

  std::vector<burukov::Polygon> polygons;
  burukov::readData(file, polygons);
  burukov::Command::polygons = &polygons;

  std::vector<burukov::Command> commands;
  std::copy(
    std::istream_iterator<burukov::Command>{std::cin},
    std::istream_iterator<burukov::Command>{},
    std::back_inserter(commands));

  return 0;
}
