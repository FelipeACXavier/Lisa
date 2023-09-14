#pragma once

#include <string>
#include <vector>

#include "lexer_tokens.h"
#include "result.h"

namespace lisa
{
class Lexer
{
public:
  Lexer();

  // No copy allowed
  Lexer(Lexer&) = delete;
  Lexer(Lexer&&) = delete;

  Result<lexer::Tokens> Parse(const std::string& filename);

  Result<lexer::Token> Next();

 private:
  const char* mBeg;

  lexer::Tokens mCurly;
  lexer::Tokens mRound;
  lexer::Tokens mSquare;

  char Get();
  char Peek() const;

  bool IsIdentifierChar(char c) const;

  VoidResult IsValid() const;

  Result<lexer::Token> Number();
  Result<lexer::Token> Identifier();
  Result<lexer::Token> SlashOrComment();
  Result<lexer::Token> Bracket(const char c);
  Result<lexer::Token> Atom(lexer::Type token);
};
}