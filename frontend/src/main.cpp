#include "dump_path_gen.hpp"
#include "lexer.hpp"
#include "my_parser.hpp"
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

    language::My_parser parser(&scanner, root, argv[1]);

    int result = parser.parse();

    if (parser.error_collector.has_errors()) {
        std::cout << "FAILED: ";
        parser.error_collector.print_errors(std::cout);
        std::cerr << "parse failed\n";
        return 1;
    }

    if (result != 0) {
        std::cerr << "unknown error\n";
        return 1;
    }

    language::Simulator simulator{};

    try {
        root->accept(simulator);
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << "\n";
        return 1;
    }

    // ____________GRAPH DUMP___________ // 
    // const auto paths = language::make_dump_paths();
    // const std::string gv_file = paths.gv.string();
    // const std::string svg_file = paths.svg.string();
    // // dot dump/dump.gv -Tsvg -o dump/dump.svg

    // std::ofstream gv(gv_file);
    // if (!gv) {
    //     std::cerr << "unable to open gv file\n";
    //     return 1;
    // }
    // root->graph_dump(gv, nullptr);
}
