#include "logging.h"
#include "parser.h"
#include "result.h"

#include <memory>

int main()
{
  std::unique_ptr<lisa::Parser> parser = std::make_unique<lisa::Parser>();

  LOG_ERROR_ON_FAILURE(parser->Parse(R"(
  func soma(a, b) {
    retorno a + b;
  }

  func inicial(a, b) {
    retorno 0;
  })"));

  return 0;
}