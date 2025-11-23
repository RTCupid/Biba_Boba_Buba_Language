find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
include(GoogleTest)

set(SRC_LIST
    src/expr_evaluator.cpp
    ${FLEX_Lexer_OUTPUTS}
)

add_executable(expr_evaluator ${SRC_LIST})

target_link_libraries(expr_evaluator
    PRIVATE 
        frontend::headers
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
        ${FLEX_LIBRARIES}
)

gtest_discover_tests(expr_evaluator
    PROPERTIES LABELS "unit"
)  