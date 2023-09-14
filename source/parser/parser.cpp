#include "ast.h"

#include "logging.h"
#include "parser.h"
#include "result.h"
#include "tokens.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <memory>


namespace lisa
{
Parser::Parser()
{
}

VoidResult Parser::Parse(const std::string &contents)
{
  // Save so contents can be accessed from all functions
  mContents = contents;

  // Start at index 0
  return Parse(0);
}

Result<uint32_t> Parser::Parse(uint32_t index)
{
  uint32_t i = index;
  std::string token = "";
  for (; i < mContents.size(); ++i)
  {
    char c = mContents.at(i);
    while (std::isalnum(c))
    {
      token += c;
      c = mContents.at(++i);
    }

    auto index = ParseToken(i, token);
    RETURN_ON_FAILURE(index);
    i = index.Value();

    LOG_INFO(token);
    token.clear();
  }

  return i;
}

Result<uint32_t> Parser::ParseToken(uint32_t index, const std::string& token)
{
  if (token.empty())
    return index;

  LOG_INFO(token);

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

  bool run = true;
  for (; i < mContents.size() && run; ++i)
  {
    const char c = mContents.at(i);
    switch (c)
    {
      case Tokens::CURLY_LEFT:
      {

        std::shared_ptr<Bloco> bloco = std::make_shared<Bloco>();
        auto ret = Parse(i);
        if (!ret.IsSuccess())
          return ret;

        i = ret.Value();

        function.block = bloco;
        break;
      }
      case Tokens::PAR_LEFT:
      {
        std::shared_ptr<Palavra> name = std::make_shared<Palavra>();
        name->value = token;
        function.name = name;

        token.clear();
        break;
      }
      case Tokens::COMMA:
      case Tokens::PAR_RIGHT:
      {
        std::shared_ptr<Palavra> arg = std::make_shared<Palavra>();
        arg->value = token;
        function.arguments.push_back(arg);

        token.clear();
        break;
      }
      case Tokens::SEMI_COLON:
      case Tokens::CURLY_RIGHT:
      {
        run = false;
        break;
      }
      case Tokens::TAB:
      case Tokens::NEW_LINE:
      case Tokens::SPACE:
        break;
      default:
        token += c;
        break;
    }
  }

  mAst.mFunctions.push_back(function);

  // mAst.Print();

  return i - 1;
}

}