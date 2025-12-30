#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_tuple_assign/tuple_assign.txt"

out=$("$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')

if [ "$norm" = "999 5 -5 8 8" ]; then
  echo "test_tuple_assign success"
  exit 0
else
  echo "test_tuple_assign fail"
  exit 1
fi
