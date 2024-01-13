#!/bin/sh

ref_file_out=/tmp/.ref_file_out
ref_file_err=/tmp/.ref_file_err
my_file_out=/tmp/.my_file_out
my_file_err=/tmp/.my_file_err

script=script.sh

# Execute the command line : ./42sh < <script.sh>
# Run your function and store the output in a file
run_test_redir()
{
    echo "$1" > $script
    # Store the actual output and stderr
    ./42sh < "$script" > "$my_file_out" 2> $my_file_err
    MY_CODE = $?
    # Store the expected output and stderr
    bash --posix "$script" > "$ref_file_out" 2> $ref_file_err
    REF_CODE = $?

    diff --color=always -u $ref_file_out $my_file_out > $1.diff
    DIFF_CODE = $?

    # Return code
    if [ $REF_CODE != $MY_CODE ]; then
        echo -ne "$RED RETURN$WHITE"
        success = false
    fi

    if [ $DIFF_CODE != 0 ]; then
        echo -ne "$RED STDOUT$WHITE"
        success = false
    fi

    if diff -q "$my_file_out" "$ref_file_out" > /dev/null; then
        echo -e "\e[32mOK\e[0m"
    else
        echo -e "\e[31mDIFFERENT:\e[0m $1"

        # Display the difference
        echo -e "\e[31mDifference:\e[0m"
        diff -u "$my_file_out" "$ref_file_out"
    fi

    # Stderr return
    if { [ -s $ref_file_err ] && [ ! -s $my_file_err ]; } ||
        { [ ! -s $ref_file_err ] && [ -s $my_file_err ]; }; then
            echo -ne "$RED STDERR$WHITE"
            success = false
    fi

    if $success; then
        echo -e "$GREEN OK$WHITE"
        rm -f $1.diff
    else
        [ -s "$(realpath $1.diff)" ]
        && echo -ne "$RED (cat $(realpath $1.diff))$WHITE"
        echo
    fi
}

# ============================= Test ECHO ====================================
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
run_test_redir "echo foo\n ;"

# Middle (\n)
run_test_redir "echo \n tata"
run_test_redir "echo '\n' tata"
run_test_redir "echo 'Hello\nWorld!'"
run_test_redir "echo Hello\nWorld!"
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

# ============================== Test IF =====================================
# if - then - else
run_test_redir "if true ; then echo foo ; else echo bar ; fi"
run_test_redir "if true ; then echo true ;else echo bar; fi"
run_test_redir "if false ; then echo foo bar ;else echo zoubir ;fi"
run_test_redir "if if true; then echo uwu; fi;then echo jambon;fi"
run_test_redir "if if false; then echo uwu; fi;then echo jambon;fi"

# if - elif
run_test_redir "if false;then echo true ; elif false; then echo true;else echo jambon;fi"


# ============================= Test ERROR ===================================
# general
run_test_redir ";"

# Rule_if
run_test_redir "if true then echo bar fi"
run_test_redir "'if' true ; then echo yes ; fi"
run_test_redir "if true ; 'then' echo yes ; fi"
run_test_redir "if if true; then echo uwu; fi; then echo jambon"
run_test_redir "if if true; then echo uwu; then echo jambon fi"

# fail execvp
#run_test_redir "echor -a toto"
#run_test_redir "ls -q src/"


# ============================== THE END =====================================
rm -f $ref_file_out $my_file_out $ref_file_err $my_file_err $script
