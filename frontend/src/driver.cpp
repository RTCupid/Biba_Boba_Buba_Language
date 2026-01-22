#include "driver.hpp"

void driver(int argc, char**& argv) {
    if (argc < 2) {
        throw std::runtime_error(std::string("Usage: ") + argv[0] + " <program_file>");
    }

    std::ifstream program_file(argv[1]);
    if (!program_file)  {
        throw std::runtime_error("Cannot open program file\n");
    }
    language::Lexer scanner(&program_file, &std::cout);

    std::unique_ptr<language::Program> root;

    language::My_parser parser(&scanner, root, argv[1]);

    int result = parser.parse();

    if (parser.error_collector.has_errors()) {
        std::cout << "FAILED: ";
        parser.error_collector.print_errors(std::cout);
        throw std::runtime_error("parse failed\n");
    }

    if (result != 0) {
        throw std::runtime_error("unknown error\n");
    }

    language::Simulator simulator{};
    root->accept(simulator);

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