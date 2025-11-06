#include <iostream>
#include <string>
#include <iterator>
#include <memory>

#include "parser.hpp"
#include "lexer.hpp"

using namespace language;

int main() {

    std::string source(
        std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>()
    );

    Lexer lexer(std::move(source));
    lexer.print_source();

    lexer.tokenize();
    lexer.print_tokens();

    std::cout << "Я работаю\n";

}
