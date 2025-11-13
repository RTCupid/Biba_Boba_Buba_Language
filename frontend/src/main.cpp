#include "lexer.hpp"
#include "node.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <memory>

int yyFlexLexer::yywrap() { return 1; }

// int main() {
//     language::Lexer scanner(&std::cin, &std::cout);
//     while (true) {
//         int tok = scanner.yylex();
//         if (tok == 0) { std::cout << "EOF\n"; break; }
//         if (tok < 0) { std::cerr << "Lex error\n"; return 1; }
//         scanner.print_current();
//     }
// }

int main() {
    std::istream *in = &std::cin;
    language::Lexer scanner(in, &std::cout);

    std::unique_ptr<language::Program> root;

    yy::parser parser(&scanner, root);

    int result = parser.parse();

    if (result == 0 && root) {
        std::cout << "Parse OK\n";
    } else {
        std::cerr << "Parse failed\n";
    }

    return result;
}
