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

  bool hasEvenVertices(const Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool hasOddVertices(const Polygon& polygon)
  {
    return polygon.points.size() % 2 != 0;
  }

  bool hasNVertices(const Polygon& polygon, size_t count)
  {
    return polygon.points.size() == count;
  }

  bool areaLess(const Polygon& lhs, const Polygon& rhs)
  {
    return getArea(lhs) < getArea(rhs);
  }

  bool verticesLess(const Polygon& lhs, const Polygon& rhs)
  {
    return lhs.points.size() < rhs.points.size();
  }

  bool isRect(const Polygon& polygon)
  {
    if (polygon.points.size() != 4)
    {
      return false;
    }

    const auto& pts = polygon.points;
    long long dx01 = static_cast<long long>(pts[1].x) - pts[0].x;
    long long dy01 = static_cast<long long>(pts[1].y) - pts[0].y;
    long long dx12 = static_cast<long long>(pts[2].x) - pts[1].x;
    long long dy12 = static_cast<long long>(pts[2].y) - pts[1].y;
    long long dx23 = static_cast<long long>(pts[3].x) - pts[2].x;
    long long dy23 = static_cast<long long>(pts[3].y) - pts[2].y;
    long long dx30 = static_cast<long long>(pts[0].x) - pts[3].x;
    long long dy30 = static_cast<long long>(pts[0].y) - pts[3].y;

    return (dx01 * dx12 + dy01 * dy12 == 0) &&
           (dx12 * dx23 + dy12 * dy23 == 0) &&
           (dx23 * dx30 + dy23 * dy30 == 0) &&
           (dx30 * dx01 + dy30 * dy01 == 0);
  }

  bool hasRightAngle(const Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return false;
    }

    size_t n = polygon.points.size();

    for (size_t i = 0; i < n; ++i)
    {
      const Point& a = polygon.points[i];
      const Point& b = polygon.points[(i + 1) % n];
      const Point& c = polygon.points[(i + 2) % n];

      long long abx = static_cast<long long>(b.x) - a.x;
      long long aby = static_cast<long long>(b.y) - a.y;
      long long bcx = static_cast<long long>(c.x) - b.x;
      long long bcy = static_cast<long long>(c.y) - b.y;

      if (abx * bcx + aby * bcy == 0)
      {
        return true;
      }
    }

    return false;
  }

  bool isRightShape(const Polygon& polygon)
  {
    return hasRightAngle(polygon);
  }
}
