#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_bitwise_op/bitwise_op.txt"

out=$("$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')

if [ "$norm" = "1 1 1 1 1 1 1 1 1" ]; then
  echo "test_bitwise_op success"
  exit 0
else
  echo "test_bitwise_op fail"
  exit 1
fi
