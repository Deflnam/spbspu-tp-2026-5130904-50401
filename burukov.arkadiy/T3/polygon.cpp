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

  bool operator==(const Point& lhs, const Point& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  void readData(std::istream& in, std::vector<Polygon>& polygons)
  {
    if (in.eof())
    {
      return;
    }

    Polygon polygon{};

    if (in >> polygon)
    {
      polygons.push_back(polygon);
      readData(in, polygons);
      return;
    }

    if (in.eof())
    {
      return;
    }

    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    readData(in, polygons);
  }

  Triangle makeTriangle(const std::vector<Point>& points, size_t index)
  {
    return Triangle{ points[0], points[index], points[index + 1] };
  }

  double getTriangleArea(const Triangle& triangle)
  {
    double first = static_cast<double>(triangle.a.x) * (triangle.b.y - triangle.c.y);
    double second = static_cast<double>(triangle.b.x) * (triangle.c.y - triangle.a.y);
    double third = static_cast<double>(triangle.c.x) * (triangle.a.y - triangle.b.y);

    return std::fabs(first + second + third) / 2.0;
  }

  double getArea(const Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      throw std::invalid_argument("polygon must have at least 3 points");
    }

    std::vector<size_t> indexes(polygon.points.size() - 2);
    std::iota(indexes.begin(), indexes.end(), 1);

    std::vector<Triangle> triangles(indexes.size());
    std::transform(
      indexes.begin(),
      indexes.end(),
      triangles.begin(),
      [&points = polygon.points](size_t i)
      {
        return makeTriangle(points, i);
      });

    std::vector<double> areas(triangles.size());
    std::transform(triangles.begin(), triangles.end(), areas.begin(), getTriangleArea);

    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
}
