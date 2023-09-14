#include "lexer.h"
#include "logging.h"
#include "parser.h"
#include "result.h"

#include <chrono>
#include <memory>

int main()
{
  std::unique_ptr<lisa::Lexer> lexer = std::make_unique<lisa::Lexer>();

  auto start = std::chrono::high_resolution_clock::now();
  auto ret = lexer->Parse("/home/felaze/Projects/Lisa/test/base.lisa");
  auto end = std::chrono::high_resolution_clock::now();

  double timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  LOG_INFO("Time taken to lex: %.2lf us", timeTaken);

  if (!ret.IsSuccess())
  {
    LOG_ERROR(ret.ErrorMessage());
    return -1;
  }

  for (const auto& token : ret.Value())
    LOG_INFO("%-12s %-10s |%s|", token.Name().c_str(), token.Loc().c_str(), token.Value().c_str());

  return 0;
}