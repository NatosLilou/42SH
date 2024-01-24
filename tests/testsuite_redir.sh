#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

echo -e "$TURQUOISE==================== Tests REDIR ===================="

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

run_test_string()
{
    CMPT=$((CMPT+1))
    # Store the actual output and stderr
    ./src/./42sh -c "$1" > "$my_file_out" 2> "$my_file_err"
    echo $? > "$my_exit_code"
    # Store the expected output and stderr
    bash --posix -c "$1" > "$ref_file_out" 2> "$ref_file_err"
    echo $? > "$ref_exit_code"

    # Check if the output file matches the expected output file
    if diff -q "$my_file_out" "$ref_file_out" > /dev/null &&
        (
            ([ -s "$my_file_err" ] && [ -s "$ref_file_err" ]) || 
            ([ ! -s "$my_file_err" ] && [ ! -s "$ref_file_err" ])
        ) &&      
        #diff -q "$my_file_err" "$ref_file_err" > /dev/null &&
        diff -q "$my_exit_code" "$ref_exit_code" > /dev/null; then

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

run_test_file()
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
        diff -q "$my_exit_code" "$ref_exit_code" > /dev/null; then

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

run_test_pipe()
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
        diff -q "$my_exit_code" "$ref_exit_code" > /dev/null; then

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

run_test_redir()
{
    echo "$1" > $script
    CMPT=$((CMPT+1))
    # Store the actual output and stderr
    ./src/./42sh < "$script" > "$my_file_out" 2> "$my_file_err"
    echo $? > "$my_exit_code"

    cat "$2" > save
    rm "$2"
    # Store the expected output and stderr
    bash --posix < "$script" > "$ref_file_out" 2> "$ref_file_err"
    echo $? > "$ref_exit_code"

    # Check if the output file matches the expected output file
    if diff -q "$my_file_out" "$ref_file_out" > /dev/null &&
        (
            ([ -s "$my_file_err" ] && [ -s "$ref_file_err" ]) || 
            ([ ! -s "$my_file_err" ] && [ ! -s "$ref_file_err" ])
        ) &&      
        diff -q "$2" save > /dev/null &&
        diff -q "$my_exit_code" "$ref_exit_code" > /dev/null; then

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

# ============================== Test STRING =================================
run_test_string "> uwu echo toto je" "uwu"
run_test_string "ls >| test jambon >| test2" "test2"
run_test_string "ls > test jambon > test2" "test2"
run_test_string "ls .. > test ;echo tonton >> test" "test"
run_test_string "ls toto > test; cat test" "test"
run_test_string "ls < test; cat test" "test"
run_test_string "1> ls | cat echo" "echo"
run_test_string "ls < test < test1" "test1"
run_test_string "ls 0> uwu" "uwu"
run_test_string "ls 1> hey" "hey"
run_test_string "uwu 2< sake" "sake"
# ============================== Test FILE ===================================
run_test_file "> uwu echo toto je" "uwu"
run_test_file "ls >| test jambon >| test2" "test2"
run_test_file "ls > test jambon > test2" "test2"
run_test_file "ls .. > test ;echo tonton >> test" "test"
run_test_file "ls toto > test; cat test" "test"
run_test_file "ls < test; cat test" "test"
run_test_file "1> ls | cat echo" "echo"
run_test_file "ls < test < test1" "test1"
run_test_file "ls 0> uwu" "uwu"
run_test_file "ls 1> hey" "hey"
run_test_file "uwu 2< sake" "sake"
# ============================== Test PIPE ===================================
run_test_pipe "> uwu echo toto je" "uwu"
run_test_pipe "ls >| test jambon >| test2" "test2"
run_test_pipe "ls > test jambon > test2" "test2"
run_test_pipe "ls .. > test ;echo tonton >> test" "test"
run_test_pipe "ls toto > test; cat test" "test"
run_test_pipe "ls < test; cat test" "test"
run_test_pipe "1> ls | cat echo" "echo"
run_test_pipe "ls < test < test1" "test1"
run_test_pipe "ls 0> uwu" "uwu"
run_test_pipe "ls 1> hey" "hey"
run_test_pipe "uwu 2< sake" "sake"
# ============================== Test REDIR ==================================
run_test_redir "> uwu echo toto je" "uwu"
run_test_redir "ls >| test jambon >| test2" "test2"
run_test_redir "ls > test jambon > test2" "test2"
run_test_redir "ls .. > test ;echo tonton >> test" "test"
run_test_redir "ls toto > test; cat test" "test"
run_test_redir "ls < test; cat test" "test"
run_test_redir "1> ls | cat echo" "echo"
run_test_redir "ls < test < test1" "test1"
run_test_redir "ls 0> uwu" "uwu"
run_test_redir "ls 1> hey" "hey"
run_test_redir "uwu 2< sake" "sake"
# ============================== THE END =====================================
rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err $script uwu test2 test1 hey test sake echo

echo -e "$GREEN Tests passed ${CMPT_SUCCEED} $BLUE|$RED Tests failed ${CMPT_FAILED} $BLUE|$YELLOW $((CMPT_SUCCEED*100/CMPT)) %$WHITE"
