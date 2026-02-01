#ifndef INCLUDE_DRIVER_HPP
#define INCLUDE_DRIVER_HPP

#include "dump_path_gen.hpp"
#include "iterative_ast_deleter.hpp"
#include "lexer.hpp"
#include "my_parser.hpp"
#include "node.hpp"
#include "parser.hpp"
#include "simulator.hpp"
#include <fstream>
#include <iostream>
#include <memory>

void driver(int argc, char **&argv);

#endif // INCLUDE_DRIVER_HPP