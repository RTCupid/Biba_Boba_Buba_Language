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

set_tests_properties(check_program_termination assign_in_expr bitwise_op input_in_condition PROPERTIES 
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    LABELS "end_to_end"
)