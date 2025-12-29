#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_assign_in_expr/assign_in_expr.txt"

out=$("$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')
[ "$norm" = "1 2 4" ]