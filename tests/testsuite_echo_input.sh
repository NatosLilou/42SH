#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

echo -e "$TURQUOISE===================== Tests ECHO INPUT  ====================="

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

run_test_input()
{
    echo "$1" > $script
    CMPT=$((CMPT+1))
    # Store the actual output and stderr
    cat "$script" | ./src/./42sh > "$my_file_out" 2> "$my_file_err"
    echo $? > "$my_exit_code"

    # Store the expected output and stderr
    cat "$script" | bash --posix > "$ref_file_out" 2> "$ref_file_err"
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

# ============================= Test INPUT ====================================
# Basic
run_test_input "echo toto"
run_test_input "echo -e toto\n"
run_test_input "echo -ne toto\n"
run_test_input "echo -ex toto"
run_test_input "echo -e -n -nE toto"
run_test_input "echo -n -x -e toto\n"
run_test_input "echo -e toto\ntata"
run_test_input "echo -eE toto\ntata"
run_test_input "echo -Ee toto\ntata"
run_test_input "echo toto\ntata#hello_world"
run_test_input "echo if then else\n"
run_test_input "echo 'echo'"
run_test_input "'echo' echo"
run_test_input "echo'' echo"
run_test_input "ec''ho echo"
run_test_input "ec'h'o echo"
run_test_input "echo echo"
run_test_input "'e''c'h''o echo"
run_test_input "echo e'c'h''o"
run_test_input "e'ch'o e'c'h''o"
run_test_input "echo foo\n ;"

# Middle (\n)
run_test_input "echo \n tata"
run_test_input "echo '\n' tata"
run_test_input "echo 'Hello\nWorld!'"

run_test_input 'echo Hello\nWorld!'

run_test_input "echo \\uwu"
run_test_input "echo \\\uwu\n"
run_test_input "echo -e uwu\\\n"
run_test_input "echo -e uwu\\n"
run_test_input "echo -e uwu\n"
run_test_input "echo uwu\\\n"
run_test_input "echo uwu\\n"
run_test_input "echo uwu\n"
run_test_input "echo \\\uwu"
run_test_input "echo u\w\u"

# Composed
run_test_input "echo toto\n echo tata\n"
run_test_input "echo -e toto\n echo -n tata\n"

# Comments and not comments
run_test_input "echo '#helloworld'#non_error"
run_test_input "echo Hello World#Comment\nParis 21\n"

# False redir
run_test_input "echo toto \> uwu"


# ============================== THE END =====================================
rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err $script $my_exit_code $ref_exit_code

echo -e "$GREEN Tests passed ${CMPT_SUCCEED} $BLUE|$RED Tests failed ${CMPT_FAILED} $BLUE|$YELLOW $((CMPT_SUCCEED*100/CMPT)) %$WHITE"
