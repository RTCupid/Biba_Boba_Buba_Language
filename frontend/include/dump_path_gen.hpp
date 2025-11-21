#ifndef DUMP_PATH_GEN_HPP
#define DUMP_PATH_GEN_HPP

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>

namespace language {

struct Dump_paths {
    std::filesystem::path gv;
    std::filesystem::path svg;
};

inline std::filesystem::path default_dump_dir() {
    if (const char *p = std::getenv("DUMP_DIR"); p && *p)
        return std::filesystem::path(p);

    std::error_code ec;
    auto cwd = std::filesystem::current_path(ec);
    if (!ec)
        return cwd / "dump";

    return std::filesystem::temp_directory_path() / "dump";
}

inline Dump_paths make_dump_paths(std::string_view basename = "dump",
                                  std::filesystem::path base = default_dump_dir()) {
    std::filesystem::create_directories(base);
    std::string s(basename);
    return {base / (s + ".gv"), base / (s + ".svg")};
}

} // namespace language

#endif // DUMP_PATH_GEN_HPP
