#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_PATH="../frontend/tests/end_to_end/test_fibonachi/fibonachi.txt"

out=$(printf "9\n" | "$PROGRAM" "$TEST_PATH")

norm=$(printf "%s" "$out" | tr -s '[:space:]' ' ' | sed 's/^ //; s/ $//')

if [ "$norm" = "34" ]; then
  echo "test_fibonachi success"
  exit 0
else
  echo "test_fibonachi fail"
  exit 1
fi
