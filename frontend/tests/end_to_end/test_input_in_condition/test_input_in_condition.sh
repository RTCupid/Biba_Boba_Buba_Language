#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_input_in_condition/input_in_condition.txt"

out=$(printf "1 4 0\n" | "$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')

if [ "$norm" = "10 10" ]; then
  echo "test_input_in_condition success"
  exit 0
else
  echo "test_input_in_condition fail"
  exit 1
fi
