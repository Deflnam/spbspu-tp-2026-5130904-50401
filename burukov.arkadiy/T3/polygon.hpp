#ifndef BURUKOV_POLYGON_HPP
#define BURUKOV_POLYGON_HPP

#include <iostream>
#include <vector>
#include <cstddef>

namespace burukov
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector<Point> points;
  };

  struct Triangle
  {
    Point a;
    Point b;
    Point c;
  };

  struct DelimiterIO
  {
    char exp;
  };

  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios<char>& s);
    ~IOGuard();

  private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags flags_;
    char fill_;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, Point& dest);
  std::istream& operator>>(std::istream& in, Polygon& dest);

  bool operator==(const Point& lhs, const Point& rhs);

  void readPointsRecursive(std::istream& in, std::vector<Point>& points, size_t count);
  void readData(std::istream& in, std::vector<Polygon>& polygons);

  Triangle makeTriangle(const std::vector<Point>& points, size_t index);
  double getTriangleArea(const Triangle& triangle);
  double getArea(const Polygon& polygon);

  bool hasEvenVertices(const Polygon& polygon);
  bool hasOddVertices(const Polygon& polygon);
  bool hasNVertices(const Polygon& polygon, size_t count);

  bool areaLess(const Polygon& lhs, const Polygon& rhs);
  bool verticesLess(const Polygon& lhs, const Polygon& rhs);

  bool isRect(const Polygon& polygon);
  bool hasRightAngle(const Polygon& polygon);
  bool isRightShape(const Polygon& polygon);
}

#endif
