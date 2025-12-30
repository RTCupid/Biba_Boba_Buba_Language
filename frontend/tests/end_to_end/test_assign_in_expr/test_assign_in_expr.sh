#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_assign_in_expr/assign_in_expr.txt"

out=$("$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')

if [ "$norm" = "1 2 4" ]; then
  echo "test_assign_in_expr success"
  exit 0
else
  echo "test_assign_in_expr fail"
  exit 1
fi