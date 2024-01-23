#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"


echo -e "$TURQUOISE==================== Tests ERROR ===================="

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

    # Store the expected output and stderr
    bash --posix < "$script" > "$ref_file_out" 2> "$ref_file_err"
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

# ============================== Test STRING =================================
# general
run_test_string ";"

# Rule_if
run_test_string "if true then echo bar fi"
run_test_string "'if' true ; then echo yes ; fi"
run_test_string "if true ; 'then' echo yes ; fi"
run_test_string "if if true; then echo uwu; fi; then echo jambon"
run_test_string "if if true; then echo uwu; then echo jambon fi"

# fail execvp
run_test_string "echor -a toto"
run_test_string "ls -q src/"

#redirection
run_test_string ">test if true echo" # Exit code: 127
run_test_string "if uwu >test if true echo" # Exit code:2

# ============================== Test FILE =================================
# general
run_test_file ";"

# Rule_if
run_test_file "if true then echo bar fi"
run_test_file "'if' true ; then echo yes ; fi"
run_test_file "if true ; 'then' echo yes ; fi"
run_test_file "if if true; then echo uwu; fi; then echo jambon"
run_test_file "if if true; then echo uwu; then echo jambon fi"

# fail execvp
run_test_file "echor -a toto"
run_test_file "ls -q src/"

#redirection
run_test_file ">test if true echo" # Exit code: 127
run_test_file "if uwu >test if true echo" # Exit code:2

# ============================== Test PIPE =================================
# general
run_test_pipe ";"

# Rule_if
run_test_pipe "if true then echo bar fi"
run_test_pipe "'if' true ; then echo yes ; fi"
run_test_pipe "if true ; 'then' echo yes ; fi"
run_test_pipe "if if true; then echo uwu; fi; then echo jambon"
run_test_pipe "if if true; then echo uwu; then echo jambon fi"

# fail execvp
run_test_pipe "echor -a toto"
run_test_pipe "ls -q src/"

#redirection
run_test_pipe ">test if true echo" # Exit code: 127
run_test_pipe "if uwu >test if true echo" # Exit code:2

# ============================== Test REDIR =================================
# general
run_test_redir ";"

# Rule_if
run_test_redir "if true then echo bar fi"
run_test_redir "'if' true ; then echo yes ; fi"
run_test_redir "if true ; 'then' echo yes ; fi"
run_test_redir "if if true; then echo uwu; fi; then echo jambon"
run_test_redir "if if true; then echo uwu; then echo jambon fi"

# fail execvp
run_test_redir "echor -a toto"
run_test_redir "ls -q src/"

#redirection
run_test_redir ">test if true echo" # Exit code: 127
run_test_redir "if uwu >test if true echo" # Exit code:2

# ============================== THE END =====================================
rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err $script $my_exit_code $ref_exit_code test

echo -e "$GREEN Tests passed ${CMPT_SUCCEED} $BLUE|$RED Tests failed ${CMPT_FAILED} $BLUE|$YELLOW $((CMPT_SUCCEED*100/CMPT)) %$WHITE"
