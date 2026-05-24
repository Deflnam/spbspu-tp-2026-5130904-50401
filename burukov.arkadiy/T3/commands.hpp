#ifndef BURUKOV_COMMANDS_HPP
#define BURUKOV_COMMANDS_HPP

#include <iosfwd>
#include <vector>
#include "polygon.hpp"

namespace burukov
{
  void area(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void perms(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void rects(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void rightShapes(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
}

#endif
