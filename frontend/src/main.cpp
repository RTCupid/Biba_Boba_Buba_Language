#include <memory>
#include <FlexLexer.h>
#include "lexer.hpp"

int yyFlexLexer::yywrap() { return 1; }

using namespace language;

int main() {
  auto lexer = std::make_unique<Lexer>();
  while (lexer->yylex() != 0) {
    lexer->print_current();
  }
}
