add_test(
    NAME check_program_termination 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/check_program_termination.sh
)

add_test(
    NAME assign_in_expr 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/test_assign_in_expr/test_assign_in_expr.sh
)

add_test(
    NAME bitwise_op 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/test_bitwise_op/test_bitwise_op.sh
)

add_test(
    NAME input_in_condition 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/test_input_in_condition/test_input_in_condition.sh
)

add_test(
    NAME input_in_expression 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/test_input_in_expression/test_input_in_expression.sh
)

add_test(
    NAME fibonachi 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/test_fibonachi/test_fibonachi.sh
)

add_test(
    NAME tuple_assign 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/test_tuple_assign/test_tuple_assign.sh
)

add_test(
    NAME logical_operators 
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash ${CMAKE_CURRENT_SOURCE_DIR}/test_logical_operators/test_logical_operators.sh
)

set_tests_properties(check_program_termination assign_in_expr bitwise_op input_in_condition input_in_expression fibonachi tuple_assign logical_operators PROPERTIES 
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    LABELS "end_to_end"
)