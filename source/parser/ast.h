#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "logging.h"

namespace lisa
{

static const std::string TAB = "\t";

struct AST;
struct Bloco;
struct Funcao;
struct Statement;
struct Expression;

struct Expression {
  virtual ~Expression() = default;

  std::string loc;

  virtual void Print(const std::string& tab) const
  {
    LOG_DEBUG("%sExpression not implemented", tab.c_str());
  }
};

struct Variavel            : Expression
{
  std::string value;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG("[Variavel]: %s", value.c_str());
  }
};
struct Inteiro             : Expression
{
  int value;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG("[Inteiro]: %d", value);
  }
};

struct Flutuante           : Expression
{
  double value;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG("[Flutuante]: %lf", value);
  }
};

struct Palavra             : Expression
{
  std::string value;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG("%s[Palavra]: %s", tab.c_str(), value.c_str());
  }
};

struct Lista               : Expression
{
  std::vector<Expression> value;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG("%s[Lista]:", tab.c_str());
    for (const auto& v : value)
      v.Print(tab + TAB);
  }
};

struct Invocar             : Expression
{
  Expression funcao;
  std::vector<Expression> argumentos;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG("%s[Invocar]:", tab.c_str());
    funcao.Print(tab + TAB);
    for (const auto& a : argumentos)
      a.Print(tab + TAB);
  }
};

// Bool
struct Negacao             : Expression { Expression lhs; };
struct Conjuncao           : Expression { Expression lhs; Expression rhs; };
struct Disjuncao           : Expression { Expression lhs; Expression rhs; };
struct Igualdade           : Expression { Expression lhs; Expression rhs; };
struct MaiorQue            : Expression { Expression lhs; Expression rhs; };
struct MenorQue            : Expression { Expression lhs; Expression rhs; };
struct MaiorOuIgual        : Expression { Expression lhs; Expression rhs; };
struct MenorOuIgual        : Expression { Expression lhs; Expression rhs; };

// Algebra
struct Adicao              : Expression { Expression lhs; Expression rhs; };
struct Subtracao           : Expression { Expression lhs; Expression rhs; };
struct Divisao             : Expression { Expression lhs; Expression rhs; };
struct Multiplicacao       : Expression { Expression lhs; Expression rhs; };

struct Statement
{
  virtual ~Statement() = default;

  std::string loc;

  virtual void Print(const std::string& tab) const
  {
    LOG_INFO("%sStatement not implemented", tab.c_str());
  }
};

struct Bloco               : Statement
{
  std::vector<std::shared_ptr<Statement>> statements;

  void Print(const std::string& tab) const
  {
    for (const auto s : statements)
      s->Print(tab + TAB);
  }
};

struct Se                  : Statement { Expression condition; Statement ifblock; std::optional<Statement> elseblock; };
struct Enquanto            : Statement { Expression condition; Bloco block; };
struct Parar               : Statement {};
struct Continuar           : Statement {};
struct Retorno             : Statement {};

struct LerVariavel         : Statement { Expression variable; };
struct LerPropriedade      : Statement { Expression classe; Expression property; std::vector<Expression> argumentos; };
struct EscreverVariavel    : Statement { Expression variable; Expression value; };
struct EscreverPropriedade : Statement { Expression classe; Expression property; Expression value; };

struct Macro
{
  std::shared_ptr<Statement> expression;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG("[Macro]:");
    expression->Print(tab + TAB);
  }
};

struct Funcao
{
  std::shared_ptr<Expression> name;
  std::shared_ptr<Bloco> block;
  std::vector<std::shared_ptr<Expression>> arguments;

  void Print(const std::string& tab) const
  {
    LOG_DEBUG(tab + "[Funcao]:");
    LOG_DEBUG(tab + "- Nome:");
    name->Print(tab + TAB + TAB);
    LOG_DEBUG(tab + "- Argumentos:");
    for (const auto a : arguments)
      a->Print(tab + TAB + TAB);
    LOG_DEBUG(tab + "- Bloco:");
    block->Print(tab + TAB + TAB);

  }
};

struct AST
{
  std::vector<Funcao> mFunctions;
  std::vector<Macro> mMacros;

  void Print() const
  {
    LOG_DEBUG("[AST]:");
    for (const auto& f : mFunctions)
      f.Print(TAB);

    for (const auto& m : mMacros)
      m.Print(TAB);
  }
};
}