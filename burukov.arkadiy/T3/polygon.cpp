#include "polygon.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <numeric>
#include <stdexcept>

namespace burukov
{
  IOGuard::IOGuard(std::basic_ios<char>& s) :
    s_(s),
    width_(s.width()),
    precision_(s.precision()),
    flags_(s.flags()),
    fill_(s.fill())
  {}

  IOGuard::~IOGuard()
  {
    s_.width(width_);
    s_.precision(precision_);
    s_.flags(flags_);
    s_.fill(fill_);
  }

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    char c = '0';
    in >> c;

    if (in && c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

  std::istream& operator>>(std::istream& in, Point& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    Point point{};
    in >> DelimiterIO{'('} >> point.x >> DelimiterIO{';'} >> point.y >> DelimiterIO{')'};

    if (in)
    {
      dest = point;
    }

    return in;
  }

  void readPointsRecursive(std::istream& in, std::vector<Point>& points, size_t count)
  {
    if (count == 0)
    {
      return;
    }

    Point point{};

    if (!(in >> point))
    {
      in.setstate(std::ios::failbit);
      return;
    }

    points.push_back(point);
    readPointsRecursive(in, points, count - 1);
  }

  std::istream& operator>>(std::istream& in, Polygon& dest)
  {
    std::istream::sentry sentry(in);

    if (!sentry)
    {
      return in;
    }

    Polygon polygon{};
    size_t count = 0;
    in >> count;

    if (!in || count < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    polygon.points.reserve(count);
    readPointsRecursive(in, polygon.points, count);

    if (in && polygon.points.size() == count)
    {
      dest = polygon;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }
}
