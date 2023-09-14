#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "logging.h"

namespace lisa
{

struct Location
{
  long line;
  long column;
};

extern Location gLocation;

namespace lexer
{

static const std::string names[]{
    "Number",      "Identifier",  "LeftParen",  "RightParen", "LeftSquare",
    "RightSquare", "LeftCurly",   "RightCurly", "LessThan",   "GreaterThan",
    "Equal",       "Plus",        "Minus",      "Asterisk",   "Slash",
    "Hash",        "Dot",         "Comma",      "Colon",      "Semicolon",
    "SingleQuote", "DoubleQuote", "Comment",    "Pipe",       "And",
    "End", "Unexpected"
};

enum class Type : int
{
  Number,
  Identifier,
  LeftParen,
  RightParen,
  LeftSquare,
  RightSquare,
  LeftCurly,
  RightCurly,
  LessThan,
  GreaterThan,
  Equal,
  Plus,
  Minus,
  Asterisk,
  Slash,
  Hash,
  Dot,
  Comma,
  Colon,
  Semicolon,
  SingleQuote,
  DoubleQuote,
  Comment,
  Pipe,
  And,
  End,
  Unexpected,
};

class Token
{
public:
  Token()
  {
  }

  Token(Type kind)
      : Token(kind, "", std::size_t(0))
  {
  }

  Token(Type kind, const char* beg, std::size_t len)
      : Token(kind, beg, beg + len)
  {
  }

  Token(Type kind, const char* beg, const char* end)
      : mId{kind}
      , mValue(beg, std::distance(beg, end))
      , mLocation{gLocation.line, std::max(gLocation.column, gLocation.column - std::distance(beg, end))}
  {
  }

  Token(const Token& token)
      : mId(token.mId)
      , mValue(token.mValue)
      , mLocation(token.mLocation)
  {
  }

  Type Id() const
  {
    return mId;
  }

  std::string Name() const
  {
    return names[(int)Id()];
  }

  std::string Value() const
  {
    return mValue;
  }

  std::string Loc() const
  {
    return std::to_string(mLocation.line) + ":" + std::to_string(mLocation.column);
  }

  bool IsOneOf(const std::vector<Type>& ones)
  {
    return std::count(ones.begin(), ones.end(), Id());
  }

  bool operator==(const Type& x) const { return Id() == x; }
  bool operator!=(const Type& x) const { return Id() != x; }
  bool operator==(const Token& x) const { return Id() == x.Id(); }
  bool operator!=(const Token& x) const { return Id() != x.Id(); }

private:
  Type        mId;
  std::string mValue;
  Location    mLocation;
};

typedef std::vector<lexer::Token> Tokens;
}
}