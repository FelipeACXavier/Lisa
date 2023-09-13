#pragma once

#include <optional>
#include <string>
#include <vector>

namespace lisa
{

struct AST;
struct Bloco;
struct Funcao;
struct Statement;
struct Expression;

struct Expression { std::string loc; };

struct Variavel            : Expression { std::string value; };
struct Inteiro             : Expression { int value; };
struct Flutuante           : Expression { double value; };
struct Palavra             : Expression { std::string value; };
struct Lista               : Expression { std::vector<Expression> value; };

struct Invocar             : Expression { Expression funcao; std::vector<Expression> argumentos; };

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

struct Statement { std::string loc; };

struct Bloco               : Statement { std::vector<Statement> statements; };

struct Se                  : Statement { Expression condition; Statement ifblock; std::optional<Statement> elseblock; };
struct Enquanto            : Statement { Expression condition; Bloco block; };
struct Parar               : Statement {};
struct Continuar           : Statement {};
struct Retorno             : Statement {};

struct LerVariavel         : Statement { Expression variable; };
struct LerPropriedade      : Statement { Expression classe; Expression property; std::vector<Expression> argumentos; };
struct EscreverVariavel    : Statement { Expression variable; Expression value; };
struct EscreverPropriedade : Statement { Expression classe; Expression property; Expression value; };

struct Macro  { Statement mExpression; };

struct Funcao { Expression name; Bloco block; std::vector<Expression> arguments; };

struct AST    { std::vector<Funcao> mFunctions; std::vector<Macro> mMacros; };
}