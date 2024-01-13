#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

ref_file_out=../ref_file_out.txt
#ref_file_err=ref_file_err.txt
my_file_out=../my_file_out.txt
#my_file_err=my_file_err.txt

# Execute the command line : ./42sh -c <string>
# Run your function and store the output in a file
run_test_string()
{
    echo -ne    "$BLUE--> ${WHITE}$1...$WHITE"

    # Store the actual output and stderr
    ./src/./42sh -c "$1" > "$my_file_out" 2> /dev/null
    # Store the expected output and stderr
    bash --posix -c "$1" > "$ref_file_out" 2> /dev/null

    # Return code
    #if [ $REF_CODE != $MY_CODE ]; then
    #    echo -ne "$RED RETURN$WHITE"
    #fi

    #if [ $DIFF_CODE != 0 ]; then
    #    echo -ne "$RED STDOUT$WHITE"
    #fi
    
    # Stderr return
    #if { [ -s $ref_file_err ] && [ ! -s $my_file_err ]; } ||
    #    { [ ! -s $ref_file_err ] && [ -s $my_file_err ]; }; then
    #        echo -ne "$RED STDERR$WHITE"
    #fi

    #if $success; then
    #    echo -e "$GREEN OK$WHITE"
    #    rm -f $1.diff
    #else
    #    echo -e "$RED Difference:$WHITE"
    #    diff -u "$my_file_out" "$ref_file_out"
    #fi
    
    # Check if the output file matches the expected output file
    if diff -q "$my_file_out" "$ref_file_out" > /dev/null; then
        echo -e "\e[32mOK\e[0m"
    else
        echo -e "\e[31mDIFFERENT:\e[0m $1"

        # Display the difference
        echo -e "\e[31mDifference:\e[0m"
        diff -u "$my_file_out" "$ref_file_out"
    fi
}

# ============================= Test ECHO ====================================
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
run_test_string "echo foo\n ;"

# Middle (\n)
run_test_string "echo \n tata"
run_test_string "echo '\n' tata"
run_test_string "echo 'Hello\nWorld!'"
run_test_string "echo Hello\nWorld!"
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

# ============================== Test IF =====================================
# if - then - else
run_test_string "if true ; then echo foo ; else echo bar ; fi"
run_test_string "if true ; then echo true ;else echo bar; fi"
run_test_string "if false ; then echo foo bar ;else echo zoubir ;fi"
run_test_string "if if true; then echo uwu; fi;then echo jambon;fi"
run_test_string "if if false; then echo uwu; fi;then echo jambon;fi"

# if - elif
run_test_string "if false;then echo true ; elif false; then echo true;else echo jambon;fi"


# ============================ Test EXECVP ===================================
run_test_string "ls"
run_test_string "ls -a"
run_test_string "cd tests/"


# ============================= Test ERROR ===================================
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

run_test_string "echo toto"
# ============================== THE END =====================================
#rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err