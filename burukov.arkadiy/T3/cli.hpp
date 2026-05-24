#ifndef BURUKOV_CLI_HPP
#define BURUKOV_CLI_HPP

#include <iosfwd>
#include <vector>
#include "polygon.hpp"

namespace burukov
{
  struct Command
  {
    static const std::vector<Polygon>* polygons;
  };

  std::istream& operator>>(std::istream& in, Command& command);
}

#endif
