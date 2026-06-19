#include "note.hpp"

#include <algorithm>
#include <iostream>

namespace burukov
{
  Note::Note(const std::string& name)
    : m_name(name)
  {}

  void Note::addLine(const std::string& text)
  {
    m_lines.push_back(text);
  }

  void Note::showLines() const
  {
    for (const auto& line : m_lines)
    {
      std::cout << line << '\n';
    }
    if (m_lines.empty())
    {
      std::cout << '\n';
    }
  }

  void Note::addLink(const std::weak_ptr<Note>& link)
  {
    auto newLink = link.lock();
    if (!newLink)
    {
      throw std::logic_error("");
    }

    for (const auto& existingWeak : m_links)
    {
      auto existing = existingWeak.lock();
      if (existing && existing == newLink)
      {
        throw std::logic_error("");
      }
    }

    m_links.push_back(link);
  }

  void Note::removeLink(const std::shared_ptr<Note>& target)
  {
    auto it = std::find_if(m_links.begin(), m_links.end(),
      [&target](const std::weak_ptr<Note>& wptr) {
        auto ptr = wptr.lock();
        return ptr && ptr == target;
      });

    if (it != m_links.end())
    {
      m_links.erase(it);
    }
  }

  void Note::showLinks() const
  {
    bool printed = false;
    for (const auto& link : m_links)
    {
      auto target = link.lock();
      if (target)
      {
        std::cout << target->getName() << '\n';
        printed = true;
      }
    }
    if (!printed)
    {
      std::cout << '\n';
    }
  }

  size_t Note::countExpired() const
  {
    size_t count = 0;

    for (const auto& link : m_links)
    {
      if (!link.lock())
      {
        ++count;
      }
    }

    return count;
  }

  void Note::clearExpired()
  {
    auto it = m_links.begin();

    while (it != m_links.end())
    {
      if (it->expired())
      {
        it = m_links.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  const std::string& Note::getName() const
  {
    return m_name;
  }
}
