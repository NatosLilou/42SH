#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

echo -e "$TURQUOISE==================== Tests ECHO ===================="

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
    # Store the actual output and stderr and exit code
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

# ============================= Test STRING ==================================
# Basic
run_test_string "echo toto"
run_test_string "echo -e toto\n"
run_test_string "echo -ne toto\n"
run_test_string "echo -ex toto"
run_test_string "echo -e -n -nE toto"
run_test_string "echo -n -x -e toto\n"
run_test_string "echo -e toto\ntata"
run_test_string "echo -eE toto\ntata"
run_test_string "echo -Ee toto\ntata"
run_test_string "echo toto\ntata#hello_world"
run_test_string "echo if then else\n"
run_test_string "echo 'echo'"
run_test_string "'echo' echo"
run_test_string "echo'' echo"
run_test_string "ec''ho echo"
run_test_string "ec'h'o echo"
run_test_string "echo echo"
run_test_string "'e''c'h''o echo"
run_test_string "echo e'c'h''o"
run_test_string "e'ch'o e'c'h''o"
run_test_string "echo foo\n ;"

# Middle (\n)
run_test_string "echo \n tata"
run_test_string "echo '\n' tata"
run_test_string "echo 'Hello\nWorld!'"

run_test_string 'echo Hello\nWorld!'

run_test_string "echo \\uwu"
run_test_string "echo \\\uwu\n"
run_test_string "echo -e uwu\\\n"
run_test_string "echo -e uwu\\n"
run_test_string "echo -e uwu\n"
run_test_string "echo uwu\\\n"
run_test_string "echo uwu\\n"
run_test_string "echo uwu\n"
run_test_string "echo \\\uwu"

# Composed
run_test_string "echo toto\n echo tata\n"
run_test_string "echo -e toto\n echo -n tata\n"

# Comments and not comments
run_test_string "echo '#helloworld'#non_error"
run_test_string "echo Hello World!#Comment\nParis 21\n"

# ============================= Test FILE ====================================
# Basic
run_test_file "echo toto"
run_test_file "echo -e toto\n"
run_test_file "echo -ne toto\n"
run_test_file "echo -ex toto"
run_test_file "echo -e -n -nE toto"
run_test_file "echo -n -x -e toto\n"
run_test_file "echo -e toto\ntata"
run_test_file "echo -eE toto\ntata"
run_test_file "echo -Ee toto\ntata"
run_test_file "echo toto\ntata#hello_world"
run_test_file "echo if then else\n"
run_test_file "echo 'echo'"
run_test_file "'echo' echo"
run_test_file "echo'' echo"
run_test_file "ec''ho echo"
run_test_file "ec'h'o echo"
run_test_file "echo echo"
run_test_file "'e''c'h''o echo"
run_test_file "echo e'c'h''o"
run_test_file "e'ch'o e'c'h''o"
run_test_file "echo foo\n ;"

# Middle (\n)
run_test_file "echo \n tata"
run_test_file "echo '\n' tata"
run_test_file "echo 'Hello\nWorld!'"

run_test_file 'echo Hello\nWorld!'

run_test_file "echo \\uwu"
run_test_file "echo \\\uwu\n"
run_test_file "echo -e uwu\\\n"
run_test_file "echo -e uwu\\n"
run_test_file "echo -e uwu\n"
run_test_file "echo uwu\\\n"
run_test_file "echo uwu\\n"
run_test_file "echo uwu\n"
run_test_file "echo \\\uwu"

# Composed
run_test_file "echo toto\n echo tata\n"
run_test_file "echo -e toto\n echo -n tata\n"

# Comments and not comments
run_test_file "echo '#helloworld'#non_error"
run_test_file "echo Hello World!#Comment\nParis 21\n"

# ============================= Test PIPE ====================================
# Basic
run_test_pipe "echo toto"
run_test_pipe "echo -e toto\n"
run_test_pipe "echo -ne toto\n"
run_test_pipe "echo -ex toto"
run_test_pipe "echo -e -n -nE toto"
run_test_pipe "echo -n -x -e toto\n"
run_test_pipe "echo -e toto\ntata"
run_test_pipe "echo -eE toto\ntata"
run_test_pipe "echo -Ee toto\ntata"
run_test_pipe "echo toto\ntata#hello_world"
run_test_pipe "echo if then else\n"
run_test_pipe "echo 'echo'"
run_test_pipe "'echo' echo"
run_test_pipe "echo'' echo"
run_test_pipe "ec''ho echo"
run_test_pipe "ec'h'o echo"
run_test_pipe "echo echo"
run_test_pipe "'e''c'h''o echo"
run_test_pipe "echo e'c'h''o"
run_test_pipe "e'ch'o e'c'h''o"
run_test_pipe "echo foo\n ;"

# Middle (\n)
run_test_pipe "echo \n tata"
run_test_pipe "echo '\n' tata"
run_test_pipe "echo 'Hello\nWorld!'"

run_test_pipe 'echo Hello\nWorld!'

run_test_pipe "echo \\uwu"
run_test_pipe "echo \\\uwu\n"
run_test_pipe "echo -e uwu\\\n"
run_test_pipe "echo -e uwu\\n"
run_test_pipe "echo -e uwu\n"
run_test_pipe "echo uwu\\\n"
run_test_pipe "echo uwu\\n"
run_test_pipe "echo uwu\n"
run_test_pipe "echo \\\uwu"

# Composed
run_test_pipe "echo toto\n echo tata\n"
run_test_pipe "echo -e toto\n echo -n tata\n"

# Comments and not comments
run_test_pipe "echo '#helloworld'#non_error"
run_test_pipe "echo Hello World!#Comment\nParis 21\n"

# ============================= Test REDIR ====================================
# Basic
run_test_redir "echo toto"
run_test_redir "echo -e toto\n"
run_test_redir "echo -ne toto\n"
run_test_redir "echo -ex toto"
run_test_redir "echo -e -n -nE toto"
run_test_redir "echo -n -x -e toto\n"
run_test_redir "echo -e toto\ntata"
run_test_redir "echo -eE toto\ntata"
run_test_redir "echo -Ee toto\ntata"
run_test_redir "echo toto\ntata#hello_world"
run_test_redir "echo if then else\n"
run_test_redir "echo 'echo'"
run_test_redir "'echo' echo"
run_test_redir "echo'' echo"
run_test_redir "ec''ho echo"
run_test_redir "ec'h'o echo"
run_test_redir "echo echo"
run_test_redir "'e''c'h''o echo"
run_test_redir "echo e'c'h''o"
run_test_redir "e'ch'o e'c'h''o"
run_test_redir "echo foo\n ;"

# Middle (\n)
run_test_redir "echo \n tata"
run_test_redir "echo '\n' tata"
run_test_redir "echo 'Hello\nWorld!'"

run_test_redir 'echo Hello\nWorld!'

run_test_redir "echo \\uwu"
run_test_redir "echo \\\uwu\n"
run_test_redir "echo -e uwu\\\n"
run_test_redir "echo -e uwu\\n"
run_test_redir "echo -e uwu\n"
run_test_redir "echo uwu\\\n"
run_test_redir "echo uwu\\n"
run_test_redir "echo uwu\n"
run_test_redir "echo \\\uwu"

# Composed
run_test_redir "echo toto\n echo tata\n"
run_test_redir "echo -e toto\n echo -n tata\n"

# Comments and not comments
run_test_redir "echo '#helloworld'#non_error"
run_test_redir "echo Hello World!#Comment\nParis 21\n"

# ============================== THE END =====================================
rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err $script $my_exit_code $ref_exit_code

echo -e "$GREEN Tests passed ${CMPT_SUCCEED} $BLUE|$RED Tests failed ${CMPT_FAILED} $BLUE|$YELLOW $((CMPT_SUCCEED*100/CMPT)) %$WHITE"
