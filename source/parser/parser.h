#pragma once

#include <string>

#include "ast.h"
#include "result.h"

namespace lisa
{
class Parser
{
public:
  Parser();

  VoidResult Parse(const std::string& contents);

private:
  std::string mContents;

  AST mAst;

  Result<uint32_t> Parse(uint32_t index);
  Result<uint32_t> ParseToken(uint32_t index, const std::string& token);

  Result<uint32_t> ParseFunction(uint32_t index);
};
}