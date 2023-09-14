#include "lexer.h"

#include <cctype>
#include <cstddef>
#include <iostream>
#include <vector>

#include "file_system_helpers.h"
#include "result.h"

using namespace lisa::lexer;

#define PARSE_LEFT_BRACKET(vec, type)   \
  do                                    \
  {                                     \
      auto token = Atom(type);          \
      if (token.IsSuccess())            \
        vec.push_back(token.Value());   \
      return token;                     \
  } while(1);

#define PARSE_RIGHT_BRACKET(vec, type, msg)                                \
  do                                                                       \
  {                                                                        \
      auto token = Atom(type);                                             \
      if (vec.empty())                                                     \
        return Result<lexer::Token>::Failed("Stray closing " +             \
                                            std::string(msg) +             \
                                            " at " + token.Value().Loc()); \
      vec.pop_back();                                                      \
      return token;                                                        \
  } while(1);

namespace lisa
{

Location gLocation{1, 1};

void ResetLocation()
{
  gLocation.line += 1;
  gLocation.column = 1;
}

Lexer::Lexer()
    : mBeg(nullptr)
{
}

char Lexer::Get()
{
  gLocation.column += 1;
  return *mBeg++;
}

char Lexer::Peek() const
{
  return *mBeg;
}

bool Lexer::IsIdentifierChar(char c) const
{
  return std::isalnum(c) || c == '_';
}

VoidResult Lexer::IsValid() const
{
  if (!mCurly.empty())
  {
    std::string message = "Stray opening curly bracket" + std::string(mCurly.size() > 1 ? "s " : " ");
    for (const auto& token : mCurly)
      message += token.Loc() + ", ";

    return VoidResult::Failed(message.substr(0, message.size() - 2));
  }

  if (!mRound.empty())
  {
    std::string message = "Stray opening parenthesis";
    for (const auto& token : mRound)
      message += token.Loc() + ", ";

    return VoidResult::Failed(message.substr(0, message.size() - 2));
  }

  if (!mSquare.empty())
  {
    std::string message = "Stray opening square bracket" + std::string(mCurly.size() > 1 ? "s " : " ");
    for (const auto& token : mSquare)
      message += token.Loc() + ", ";

    return VoidResult::Failed(message.substr(0, message.size() - 2));
  }

  return VoidResult();
}

Result<lexer::Tokens> Lexer::Parse(const std::string& filename)
{
  if (!DoesFileExist(filename))
    return Result<lexer::Tokens>::Failed("O arquivo " + filename + " não existe");

  LOG_INFO("Passando pelo arquivo: %s", filename.c_str());

  std::string content;
  ASSIGN_OR_RETURN_ON_FAILURE_AS(content, GetFileContents(filename), lexer::Tokens);

  mBeg = content.c_str();

  lexer::Tokens tokens;
  Result<lexer::Token> result = Next();
  while (result.IsSuccess())
  {
    auto token = result.Value();
    if (token.IsOneOf({Type::End}))
      break;

    if (token == Type::Unexpected)
      return Result<lexer::Tokens>::Failed("Erro, token não reconhecido: " + token.Value() + " at " + token.Loc());

    tokens.push_back(token);

    result = Next();
  }

  if (!result.IsSuccess())
    return result.As<lexer::Tokens>();

  auto valid = IsValid();
  if (!valid)
    return Result<lexer::Tokens>::Failed("Parse error: " + valid.ErrorMessage());

  return tokens;
}

Result<lexer::Token> Lexer::Atom(lexer::Type kind)
{
  gLocation.column += 1;
  return Token(kind, mBeg++, 1);
}

Result<lexer::Token> Lexer::Next()
{
  while (std::isspace(Peek()))
  {
    char c = Peek();
    Get();
    if (c == '\n')
      ResetLocation();
  }

  const char c = Peek();
  if (c == '\0')
    return Token(Type::End, mBeg, 1);
  else if (std::isalpha(c))
    return Identifier();
  else if (std::isdigit(c))
    return Number();
  else if (c == '<')
    return Atom(Type::LessThan);
  else if (c == '>')
    return Atom(Type::GreaterThan);
  else if (c == '=')
    return Atom(Type::Equal);
  else if (c == '+')
    return Atom(Type::Plus);
  else if (c == '-')
    return Atom(Type::Minus);
  else if (c == '*')
    return Atom(Type::Asterisk);
  else if (c == '/')
    return SlashOrComment();
  else if (c == '#')
    return Atom(Type::Hash);
  else if (c == '.')
    return Atom(Type::Dot);
  else if (c == ',')
    return Atom(Type::Comma);
  else if (c == ':')
    return Atom(Type::Colon);
  else if (c == ';')
    return Atom(Type::Semicolon);
  else if (c == '\'')
    return Atom(Type::SingleQuote);
  else if (c == '"')
    return Atom(Type::DoubleQuote);
  else if (c == '|')
    return Atom(Type::Pipe);
  else if (c == '&')
    return Atom(Type::And);
  else
    return Bracket(c);
}

Result<lexer::Token> Lexer::Bracket(const char c)
{
  switch (c)
  {
    case '(':
      PARSE_LEFT_BRACKET(mRound, Type::LeftParen);
    case ')':
      PARSE_RIGHT_BRACKET(mRound, Type::RightParen, "parenthesis");
    case '[':
      PARSE_LEFT_BRACKET(mSquare, Type::LeftSquare);
    case ']':
      PARSE_RIGHT_BRACKET(mSquare, Type::RightSquare, "square bracket");
    case '{':
      PARSE_LEFT_BRACKET(mCurly, Type::LeftCurly);
    case '}':
      PARSE_RIGHT_BRACKET(mCurly, Type::RightCurly, "curly bracket");
    default:
      return Atom(Type::Unexpected);
  }
}

Result<lexer::Token> Lexer::Identifier()
{
  const char* start = mBeg;
  do
  {
    Get();
  } while (IsIdentifierChar(Peek()));

  return Token(Type::Identifier, start, mBeg);
}

Result<lexer::Token> Lexer::Number()
{
  const char* start = mBeg;
  do
  {
    Get();
  } while (std::isdigit(Peek()));

  return Token(Type::Number, start, mBeg);
}

Result<lexer::Token> Lexer::SlashOrComment()
{
  const char* start = mBeg;
  Get();
  if (Peek() == '/')
  {
    Get();
    start = mBeg;
    LOG_INFO("%c - %c", Peek(), *start);

    while (Peek() != '\0')
    {
      if (Get() == '\n')
      {
        auto token = Token(Type::Comment, start, std::distance(start, mBeg) - 1);
        ResetLocation();
        return token;
      }
    }

    return Token(Type::Unexpected, mBeg, 1);
  }
  else if (Peek() == '*')
  {
    Get();
    start = mBeg;

    while (Peek() != '\0' && !(*mBeg == '*' && *(mBeg + 1) == '/'))
    {
      // Block comments accept new lines
      if (Get() == '\n')
        ResetLocation();
    }

    if (Peek() == '\0')
      return Result<Token>::Failed("Error: reached end of file");

    // Compensate for the early check
    Get();
    Get();

    return Token(Type::Comment, start, std::distance(start, mBeg) - 2);

  }

  return Token(Type::Slash, start, 1);
}
}