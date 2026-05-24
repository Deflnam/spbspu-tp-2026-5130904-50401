#include "cli.hpp"

#include "commands.hpp"
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

const std::vector<burukov::Polygon>* burukov::Command::polygons = nullptr;

std::istream& burukov::operator>>(std::istream& in, Command&)
{
  std::string name;
  in >> name;

  if (!in)
  {
    return in;
  }

  static std::unordered_map<std::string,
      std::function<void(std::istream&, std::ostream&, const std::vector<Polygon>&)>> commands;

  if (commands.empty())
  {
    commands["AREA"] = area;
    commands["MAX"] = max;
    commands["MIN"] = min;
    commands["COUNT"] = count;
    commands["PERMS"] = perms;
    commands["RECTS"] = rects;
    commands["RIGHTSHAPES"] = rightShapes;
  }

  try
  {
    commands.at(name)(in, std::cout, *Command::polygons);
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>" << '\n';
    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return in;
}
