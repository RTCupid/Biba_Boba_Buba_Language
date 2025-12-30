#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_input_in_expression/input_in_expression.txt"

out=$(printf "1 14 3\n" | "$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')

if [ "$norm" = "12" ]; then
  echo "test_input_in_expression success"
  exit 0
else
  echo "test_input_in_expression fail"
  exit 1
fi
