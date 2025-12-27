find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
include(GoogleTest)

set(SRC_LIST
    src/process.cpp
    src/get_line_and_column.cpp
    ${FLEX_Lexer_OUTPUTS}
)

set_source_files_properties(
    ${FLEX_Lexer_OUTPUTS}
    PROPERTIES GENERATED TRUE
)

add_executable(lexer ${SRC_LIST})

add_dependencies(lexer generate_parser)

target_link_libraries(lexer
    PRIVATE 
        frontend::headers
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
        #${FLEX_LIBRARIES}
)

gtest_discover_tests(lexer
    PROPERTIES LABELS "unit"
)  