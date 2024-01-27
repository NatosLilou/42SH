#!/bin/sh

script=script.sh

run_test()
{
    echo "$1" > $script
    ./src/./42sh "$script" > /dev/null
}

# ============================== Test ========================================
run_test "echo $$"
run_test "echo $@"
run_test "echo $RANDOM"
run_test "export tata=uwu"
run_test "export tata"
run_test "tata=toto; echo \$tata; tata() { if (echo \$tata); then echo \$tata; fi }; tata"
run_test "tat=tot; tat=uwu; while false; do echo \$tat; done; until true; do echo \$tat; done"

