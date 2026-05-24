#include "datastruct.hpp"
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>

namespace burukov
{
  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios<char>& s):
      s_(s),
      precision_(s.precision()),
      width_(s.width()),
      flags_(s.flags()),
      fill_(s.fill())
    {}

    ~IOGuard()
    {
      s_.precision(precision_);
      s_.width(width_);
      s_.flags(flags_);
      s_.fill(fill_);
    }

  private:
    std::basic_ios<char>& s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios<char>::fmtflags flags_;
    char fill_;
  };

  struct DelimiterIO
  {
    char exp;
  };

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

  struct UllHexIO
  {
    unsigned long long& ref;
  };

  std::istream& operator>>(std::istream& in, UllHexIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char zero = '0';
    char x = '0';
    in >> zero >> x;
    if (!in)
    {
      return in;
    }
    if (zero != '0' || (x != 'x' && x != 'X'))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> std::hex >> dest.ref;
    return in;
  }

  struct CmpLspIO
  {
    std::complex<double>& ref;
  };

  std::istream& operator>>(std::istream& in, CmpLspIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char hash = '0';
    char c = '0';
    char open = '0';
    in >> hash >> c >> open;
    if (!in)
    {
      return in;
    }
    if (hash != '#' || c != 'c' || open != '(')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    double real = 0.0;
    double imag = 0.0;
    in >> real >> imag;
    char close = '0';
    in >> close;
    if (close != ')')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    dest.ref = std::complex<double>(real, imag);
    return in;
  }

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  }

  namespace
  {
    bool markField(std::istream& in, bool& hasKey)
    {
      if (hasKey)
      {
        in.setstate(std::ios::failbit);
        return false;
      }
      hasKey = true;
      return true;
    }
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    DataStruct input{};
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    in >> DelimiterIO{'('} >> DelimiterIO{':'};
    if (!in)
    {
      return in;
    }

    while (in && in.peek() != ')')
    {
      std::string label;
      in >> label;
      if (!in)
      {
        break;
      }

      if (label == "key1")
      {
        if (markField(in, hasKey1))
        {
          in >> UllHexIO{input.key1};
        }
      }
      else if (label == "key2")
      {
        if (markField(in, hasKey2))
        {
          in >> CmpLspIO{input.key2};
        }
      }
      else if (label == "key3")
      {
        if (markField(in, hasKey3))
        {
          in >> StringIO{input.key3};
        }
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }

      in >> DelimiterIO{':'};
    }

    in >> DelimiterIO{')'};

    if (in && hasKey1 && hasKey2 && hasKey3)
    {
      dest = input;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }

    IOGuard guard(out);

    out << "(:key1 ";
    if (dest.key1 == 0)
    {
      out << "0x0";
    }
    else
    {
      out << "0x" << std::hex << std::uppercase << dest.key1;
    }
    out << ':';

    out << std::fixed << std::setprecision(1);
    out << "key2 #c(" << dest.key2.real() << ' ' << dest.key2.imag() << "):";

    out << "key3 \"" << dest.key3 << "\":)";

    return out;
  }

  bool operator<(const DataStruct& lhs, const DataStruct& rhs)
  {
    const double epsilon = 1e-10;

    if (lhs.key1 != rhs.key1)
    {
      return lhs.key1 < rhs.key1;
    }

    const double lhsAbs = std::abs(lhs.key2);
    const double rhsAbs = std::abs(rhs.key2);

    if (std::abs(lhsAbs - rhsAbs) > epsilon)
    {
      return lhsAbs < rhsAbs;
    }

    return lhs.key3.length() < rhs.key3.length();
  }
}
