#!/bin/sh

# Function output file
function_output_file="output.txt"

# Expected output file
expected_output_file="expected_output.txt"


check1()
{
    ./main -c "$1" > "$function_output_file"
    echo -n "$2" > "$expected_output_file"
if diff -q "$function_output_file" "$expected_output_file" > /dev/null; then
    echo "\e[32mOK"
else
    echo "\e[31mDIFFERENT:\e[0m $1"

    # Display the difference
    echo "\e[31mDifference:\e[0m"
    diff -u "$function_output_file" "$expected_output_file"
fi
}

check2()
{
    ./main "$1" | cat > "$function_output_file"
    cat "$1" > "$expected_output_file"
if diff -q "$function_output_file" "$expected_output_file" > /dev/null; then
    echo "\e[32mOK"
else
    echo "\e[31mDIFFERENT:\e[0m $1"

    # Display the difference
    echo "\e[31mDifference:\e[0m"
    diff -u "$function_output_file" "$expected_output_file"
fi
}

check3()
{
    cat "$1" | ./main  > "$function_output_file"
    cat "$1" > "$expected_output_file"
if diff -q "$function_output_file" "$expected_output_file" > /dev/null; then
    echo "\e[32mOK"
else    
    echo "\e[31mDIFFERENT:\e[0m $1"

    # Display the difference
    echo "\e[31mDifference:\e[0m"
    diff -u "$function_output_file" "$expected_output_file"
fi
}
check1 "test" "test"
check2 "echo.sh"
check3 "echo.sh"