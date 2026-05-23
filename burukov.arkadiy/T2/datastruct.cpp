#include "datastruct.hpp"

namespace burukov
{
  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    return out;
  }

  bool operator<(const DataStruct& lhs, const DataStruct& rhs)
  {
    return false;
  }
}
