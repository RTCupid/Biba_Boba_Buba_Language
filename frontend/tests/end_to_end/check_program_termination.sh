#!/bin/bash

PROGRAM="./frontend/frontend"
TEST_DIR_1="../frontend/tests/end_to_end/correct_program_tests"
TEST_DIR_2="../frontend/tests/end_to_end/tests_that_do_not_compile"
LOG_FILE="termination_check.log"

# Clear the log file
> "$LOG_FILE"

echo "Testing program termination for: $PROGRAM"
echo "Test files from directory: $TEST_DIR_1"
echo "Test log will be written to: $LOG_FILE"
echo "----------------------------------------"

success_count=0
total_count=0

# Iterate over all .txt files in the directory
for test_file in "$TEST_DIR_1"/*.txt; do
    # Check if at least one .txt file exists
    if [[ ! -f "$test_file" ]]; then
        continue
    fi

    total_count=$((total_count + 1))
    filename=$(basename "$test_file")

    echo -n "Testing file: $filename ... "

    # Run the program with a 10-second timeout
    timeout 10 "$PROGRAM" "$test_file" > /dev/null 2>&1

    # Check the exit code
    exit_code=$?

    if [ $exit_code -eq 0 ]; then
        echo "SUCCESS"
        echo "$(date): $filename - SUCCESS (exit code: $exit_code)" >> "$LOG_FILE"
        success_count=$((success_count + 1))
    elif [ $exit_code -eq 124 ]; then
        echo "TIMEOUT"
        echo "$(date): $filename - TIMEOUT (program did not terminate within 10 seconds)" >> "$LOG_FILE"
    else
        echo "SEGFAULT/ERROR (exit code: $exit_code)"
        echo "$(date): $filename - ERROR (exit code: $exit_code)" >> "$LOG_FILE"
    fi
done

for test_file in "$TEST_DIR_2"/*.txt; do
    # Check if at least one .txt file exists
    if [[ ! -f "$test_file" ]]; then
        continue
    fi

    total_count=$((total_count + 1))
    filename=$(basename "$test_file")

    echo -n "Testing file: $filename ... "

    # Run the program with a 10-second timeout
    timeout 10 "$PROGRAM" "$test_file" > /dev/null 2>&1

    # Check the exit code
    exit_code=$?

    
    if [ $exit_code -eq 124 ]; then
        echo "TIMEOUT"
        echo "$(date): $filename - TIMEOUT (program did not terminate within 10 seconds)" >> "$LOG_FILE"
    elif [ $exit_code -ne 0 ]; then
        echo "SUCCESS"
        echo "$(date): $filename - SUCCESS (exit code: $exit_code)" >> "$LOG_FILE"
        success_count=$((success_count + 1))
    else
        echo "ERROR (exit code: $exit_code)"
        echo "$(date): $filename - ERROR (exit code: $exit_code)" >> "$LOG_FILE"
    fi
done

echo "----------------------------------------"
echo "Successful tests: $success_count out of $total_count files"

if [ $success_count -eq $total_count ] && [ $total_count -gt 0 ]; then
    echo "All tests passed successfully!"
    exit 0
else
    echo "Some tests failed"
    exit 1
fi

if [ $total_count -eq 0 ]; then
    echo "No .txt files found in the test directory for testing."
fi
