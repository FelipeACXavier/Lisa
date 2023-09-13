#include "ast.h"

#include "logging.h"
#include "parser.h"
#include "result.h"
#include "tokens.h"
#include <cstdint>


namespace lisa
{
Parser::Parser()
{
}

VoidResult Parser::Parse(const std::string &contents)
{
  // Save so contents can be accessed from all functions
  mContents = contents;

  std::string token = "";
  for (uint32_t i = 0; i < mContents.size(); ++i)
  {
    const char c = mContents.at(i);
    LOG_DEBUG("%c - 0x%x", c, c);
    switch (c)
    {
      case Tokens::CURLY_LEFT:
      case Tokens::CURLY_RIGHT:
        break;
      case Tokens::PAR_LEFT:
      case Tokens::PAR_RIGHT:
        break;
      case Tokens::COMMA:
      case Tokens::SEMI_COLON:
        break;
      case Tokens::SPACE:
      case Tokens::NEW_LINE:
      case Tokens::TAB:
      {
        auto index = ParseToken(i, token);
        RETURN_ON_FAILURE(index);
        i = index.Value();
        token.clear();
        break;
      }
      default:
        token += c;
    }
  }

  return VoidResult();
}

Result<uint32_t> Parser::ParseToken(uint32_t index, const std::string& token)
{
  if (token.empty())
    return index;

  Result<uint32_t> ret = Result<uint32_t>::Failed("Unknown type");
  if (token == Tokens::FUNCTION)
    ret = ParseFunction(index);

  return ret;
}

Result<uint32_t> Parser::ParseFunction(uint32_t index)
{
  uint32_t i = index;
  std::string token = "";

  Funcao function;

  for (; i < mContents.size(); ++i)
  {
    const char c = mContents.at(i);
    switch (c)
    {
      case Tokens::CURLY_LEFT:
        break;
      case Tokens::CURLY_RIGHT:
        return i;
      case Tokens::PAR_LEFT:
      {
        LOG_INFO("Function name: %s", token.c_str());

        Palavra name;
        name.value = token;
        function.name = name;

        token.clear();
        break;
      }
      case Tokens::COMMA:
      case Tokens::PAR_RIGHT:
      {
        LOG_INFO("Function argument: %s", token.c_str());

        Palavra arg;
        arg.value = token;
        function.arguments.push_back(arg);

        token.clear();
        break;
      }
      case Tokens::SEMI_COLON:
        break;
      case Tokens::SPACE:
      case Tokens::NEW_LINE:
      case Tokens::TAB:
        break;
      default:
        token += c;
        break;
    }
  }

  mAst.mFunctions.push_back(function);

  return i - 1;
}

}