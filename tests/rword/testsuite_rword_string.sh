#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

echo -e "$TURQUOISE Tests RWORD STRING"
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
    CMPT=$((CMPT+1))
    
    env -i ./src/./42sh -c "$1" > "$my_file_out" 2> "$my_file_err"
    echo $? > "$my_exit_code"
    
    if [ -f "$2" ] ; then cat "$2" > save ; else rm -f save; fi
    rm -f "$2"  

    env -i bash --posix -c "$1" > "$ref_file_out" 2> "$ref_file_err"
    echo $? > "$ref_exit_code"

    # Check if the output file matches the expected output file
    if diff -q "$my_file_out" "$ref_file_out" > /dev/null &&
        (
            ([ -s "$my_file_err" ] && [ -s "$ref_file_err" ]) || 
            ([ ! -s "$my_file_err" ] && [ ! -s "$ref_file_err" ])
        ) &&  
        (
            ([ ! -f "$2" ] && [ ! -f save ]) || 
            ([ -f "$2" ] && [ -f save ] && diff -q "$2" save > /dev/null)
        ) &&      
        diff -q "$my_exit_code" "$ref_exit_code" > /dev/null; then

        CMPT_SUCCEED=$((CMPT_SUCCEED+1))
    else
        CMPT_FAILED=$((CMPT_FAILED+1))
        echo -ne "$BLUE Test ${CMPT}... $WHITE"
        echo -e "\e[31mFAIL:\e[0m $1"

        # Display the difference
        diff -u --label "STDOUT 42SH" "$my_file_out" --label "STDOUT REF" "$ref_file_out";
        diff -u --label "STDERR 42SH" "$my_file_err" --label "STDERR REF" "$ref_file_err";
        diff -u --label "CODE 42SH" "$my_exit_code" --label "CODE REF" "$ref_exit_code";
        diff -N -u --label "REDIR 42SH" save --label "REDIR REF" "$2"; 
    fi
    rm -f "$2"
}

# ============================== Test STRING =================================

echo uwu > test
run_test "> uwu echo toto je" "uwu"
run_test "ls < test < test1" "test1"
run_test "echo toto 0> uwu" "uwu"
run_test "echo toto 1> hey" "hey"
run_test "uwu 2< sake" "sake"
run_test 'echo "$PWD" > uwu' "uwu"
run_test "echo 6<> test >&6" "uwu"
run_test "cat < test 3< jambon" "uwu"

# ============================== THE END =====================================
rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err $script uwu test2 test1 hey test sake echo save ls

echo -e "$GREEN Tests passed ${CMPT_SUCCEED} $BLUE|$RED Tests failed ${CMPT_FAILED} $BLUE|$YELLOW $((CMPT_SUCCEED*100/CMPT)) %$WHITE"
