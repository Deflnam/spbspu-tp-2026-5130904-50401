#ifndef BURUKOV_NOTE_HPP
#define BURUKOV_NOTE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace burukov
{
  class Note
  {
  public:
    explicit Note(const std::string& name);

    void addLine(const std::string& text);
    void showLines() const;
    void addLink(const std::weak_ptr<Note>& link);
    void removeLink(const std::shared_ptr<Note>& target);
    void showLinks() const;
    size_t countExpired() const;
    void clearExpired();

    const std::string& getName() const;

  private:
    std::string m_name;
    std::vector<std::string> m_lines;
    std::vector<std::weak_ptr<Note>> m_links;
  };

  using NotesMap = std::unordered_map<std::string, std::shared_ptr<Note>>;
}

#endif
