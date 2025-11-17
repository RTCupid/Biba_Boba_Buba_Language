#include "lexer.hpp"
#include "node.hpp"
#include "parser.hpp"
#include "simulator.hpp"
#include <fstream>
#include <iostream>
#include <memory>

extern int yylex();
yy::parser::semantic_type *yylval = nullptr;

int yyFlexLexer::yywrap() { return 1; }

int main(int argc, char *argv[]) {
    // std::cout << "Run program\n";
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program_file>\n";
        return 1;
    }

    std::ifstream program_file(argv[1]);
    if (!program_file) {
        std::cerr << "Cannot open program file\n";
        return 1;
    }

    language::Lexer scanner(&program_file, &std::cout);

    std::unique_ptr<language::Program> root;

    yy::parser parser(&scanner, root);

    int result = parser.parse();

    if (result != 0 || !root) {
        std::cerr << "Parse failed\n";
        return 1;
    }

    language::Simulator simulator{};

    try {
        root->accept(simulator);
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << "\n";
        return 1;
    }

    const std::string gv_file = "graph_dump/graph_dump.gv";   // FIXME delete hardcode of pathes
    const std::string svg_file = "graph_dump/graph_dump.svg"; // FIXME delete hardcode of pathes
    // dot graph_dump/graph_dump.gv -Tsvg -o graph_dump/graph_dump.svg

    std::ofstream gv(gv_file);
    if (!gv) { // FIXME it is fast implementation for debug
        std::cerr << "unable to open gv file\n";
        return 1;
    }
    root->graph_dump(gv, nullptr);

    // const auto statements = root->get_stmts();

    // Simulator simulator;

    // for (const auto & stmt : statements) {
    //     stmt->accept(simulator);
    // }

    return result;
}
