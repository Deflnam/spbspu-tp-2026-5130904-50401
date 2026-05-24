#include "commands.hpp"

#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>

int main()
{
  using namespace burukov;

  NotesMap notes;

  std::unordered_map<std::string, void (*)(std::istream&, std::ostream&, NotesMap&)> cmds;

  cmds["note"] = note;
  cmds["line"] = line;
  cmds["show"] = show;
  cmds["drop"] = drop;
  cmds["link"] = link;
  cmds["mind"] = mind;
  cmds["halt"] = halt;
  cmds["expired"] = expired;
  cmds["refresh"] = refresh;

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      auto it = cmds.find(cmd);
      if (it != cmds.end())
      {
        it->second(std::cin, std::cout, notes);
      }
      else
      {
        throw std::logic_error("");
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}
