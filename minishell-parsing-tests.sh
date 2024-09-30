#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to run a test case
run_test() {
    local test_name="$1"
    local input="$2"
    local expected_output="$3"

    echo "Running test: $test_name"
    echo "Input: $input"
    echo "Expected output: $expected_output"
    
    # Run your minishell with the input and capture only the actual output
    actual_output=$(echo "$input" | ./minishell 2>&1 | grep -v '^Minishell >' | grep -v '^input string' | grep -v '^RESULT' | grep -v '^Command\[' | sed '/^$/d')
    
    # Compare actual output with expected output
    if [ "$actual_output" == "$expected_output" ]; then
        echo -e "${GREEN}Test passed!${NC}"
    else
        echo -e "${RED}Test failed.${NC}"
        echo "Actual output: $actual_output"
    fi
    echo "----------------------------------------"
}

# Test cases
run_test "Simple echo" 'echo Hello' 'Hello'

run_test "Echo with pipe" 'echo "|"|echo s' 's'

run_test "Echo with single quotes" "echo '|'| echo a" 'a'

run_test "Echo with double quotes" 'echo "s"' 's'

run_test "Echo with environment variable" 'echo $USER' "$USER"

run_test "Echo with quoted environment variable" 'echo "$USER"' "$USER"

run_test "Echo with single-quoted environment variable" "echo '\$USER'" '$USER'

run_test "Echo with dollar single quotes" "echo \$'USER'" 'USER'

run_test "Echo with empty double quotes" 'echo ""' ''

run_test "Echo with pipe character in quotes" 'echo "|C"' '|C'

run_test "Echo with mixed quotes" "echo \"'\"'s'\"'\"" "'s'"

run_test "Echo with reversed mixed quotes" "echo '\"'\"s\"'\"'" '"s"'

run_test "Echo with multiple arguments" 'echo Hello World' 'Hello World'

run_test "Echo with multiple spaces" 'echo    Hello    World' 'Hello World'

run_test "Echo with nonexistent environment variable" 'echo $NONEXISTENT' ''

run_test "Echo with partial environment variable" 'echo $USE' ''

run_test "Echo with dollar sign" 'echo $' '$'

run_test "Echo with multiple pipes" 'echo a | echo b | echo c' 'c'

# Add more test cases as needed

echo "All tests completed."
