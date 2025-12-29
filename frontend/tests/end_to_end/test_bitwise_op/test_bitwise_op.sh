#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_bitwise_op/bitwise_op.txt"

out=$("$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')
[ "$norm" = "1 1 1 1 1 1 1 1 1" ]