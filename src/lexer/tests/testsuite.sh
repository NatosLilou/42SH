#!/bin/sh

# Function output file
function_output_file="output.txt"

# Expected output file
expected_output_file="expected_output.txt"

# Run your function and store the output in a file
run_test()
{
    # Store the actual output
    ./lexer "$1" > "$function_output_file"

    # Store the expected output
    echo -en "$2" > "$expected_output_file"

# Check if the output file matches the expected output file
if diff -q "$function_output_file" "$expected_output_file" > /dev/null; then
    echo -e "\e[32mOK"
else
    echo -e "\e[31mDIFFERENT:\e[0m $1"

    # Display the difference
    echo -e "\e[31mDifference:\e[0m"
    diff -u "$function_output_file" "$expected_output_file"
fi
}

run_test "echo toto" "WORD echo\nWORD toto\nEOF\n"
run_test "ls -a ; echo bar" "WORD ls\nWORD -a\nDELIMITER ;\nWORD echo\nWORD bar\nEOF\n"

rm -f "output.txt" "expected_output.txt"
