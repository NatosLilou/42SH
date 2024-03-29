#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

echo -e "$TURQUOISE Tests BLOCK FILE"
echo -e "$TURQUOISE =========================================================="

CMPT_TEST=0
CMPT_SUCCEED=0
CMPT_FAILED=0

ref_file_out=ref_file_out.txt
ref_file_err=ref_file_err.txt
my_file_out=my_file_out.txt
my_file_err=my_file_err.txt

my_exit_code=my_code.txt
ref_exit_code=ref_code.txt
script=script.sh

run_test()
{
    echo "$1" > $script
    CMPT=$((CMPT+1))
    # Store the actual output and stderr
    ./src/./42sh "$script" > "$my_file_out" 2> "$my_file_err"
    echo $? > "$my_exit_code"
    
    # Store the expected output and stderr
    bash --posix "$script" > "$ref_file_out" 2> "$ref_file_err"
    echo $? > "$ref_exit_code"

    # Check if the output file matches the expected output file
    if diff -q "$my_file_out" "$ref_file_out" > /dev/null &&
        (
            ([ -s "$my_file_err" ] && [ -s "$ref_file_err" ]) || 
            ([ ! -s "$my_file_err" ] && [ ! -s "$ref_file_err" ])
        ) &&
        #diff -q "$my_file_err" "$ref_file_err" > /dev/null &&
        diff -q "$my_exit_code" "$ref_exit_code" > /dev/null ; then

        #echo -ne "$BLUE Test ${CMPT}... $WHITE"
        #echo -e "\e[32mOK\e[0m";
        CMPT_SUCCEED=$((CMPT_SUCCEED+1))
    else
        CMPT_FAILED=$((CMPT_FAILED+1))
        echo -ne "$BLUE Test ${CMPT}... $WHITE"
        echo -e "\e[31mFAIL:\e[0m $1"

        # Display the difference
        diff -u --label "STDOUT 42SH" "$my_file_out" --label "STDOUT REF" "$ref_file_out";
        diff -u --label "STDERR 42SH" "$my_file_err" --label "STDERR REF" "$ref_file_err";
        diff -u --label "CODE 42SH" "$my_exit_code" --label "CODE REF" "$ref_exit_code";
    fi
}

# ============================= Test FILE ====================================
run_test 'foo() { echo this is inside a command block; }'
run_test 'foo'
run_test 'foo=too'
run_test 'foo'
run_test 'echo $foo'
run_test 'echo foo'

# 2nd sequence
run_test 'toto=tata'
run_test '$toto() { echo this is inside a command block; }'
run_test '\$toto() { echo this is inside a command block; }'
run_test 'toto=echo'
run_test '$toto r'

#3rd sequence
run_test 'foo() { echo this is inside a command block; }'
run_test 'toto=foo'
run_test '$toto'
run_test 'bar() { echo uwu; }'
run_test 'tata=bar'
run_test '$toto $tata'
run_test '$tata'
run_test '$tata $toto'
run_test '$tata ;$toto'
run_test '$tata echo i ;$toto'
run_test 'echo $toto'

#4th sequence
run_test 'foo() { echo $1; }'
run_test '$tata $toto'
run_test '$toto'
run_test '$toto uu'
run_test '$toto $tata'
run_test 'foo() { titi=uwu; }'
run_test 'titi=zeub'
run_test 'foo'
run_test 'echo $titi'
run_test 'echo $#'
run_test 'foo() { echo $#; }'
run_test 'foo r h s'
run_test 'foo() { echo toto; }'
run_test 'foo'
run_test 'tata=foo'
run_test '$tata'

# ============================== THE END =====================================
rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err $script $my_exit_code $ref_exit_code

echo -e "$GREEN Tests passed ${CMPT_SUCCEED} $BLUE|$RED Tests failed ${CMPT_FAILED} $BLUE|$YELLOW $((CMPT_SUCCEED*100/CMPT)) %$WHITE"
