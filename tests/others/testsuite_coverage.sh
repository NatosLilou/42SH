#!/bin/sh

script=script.sh

run_test()
{
    echo "$1" > $script
    ./src/./42sh "$script" uwu > /dev/null
}

# ============================== Test ========================================
run_test 'tata=toto; echo $tata; tata() { if (echo $tata); then echo $tata; fi }; tata'
run_test 'tat=tot; tat=uwu; while false; do echo $tat; done; until true; do echo $tat; done'

run_test 'echo $$'
run_test 'echo $RANDOM'
run_test 'echo ${RANDOM}'
run_test 'echo $IFS'

run_test '{ echo toto; } > test'
run_test '{ echo toto; } < test'
run_test '{ echo toto; } >> test'
run_test '{ echo toto; } <& test'
run_test '{ echo toto; } <> test'
run_test '{ echo -ne; } >& test'

touch jambon
touch toto
touch test
touch echo

run_test "ls >| test jambon >| test2"
run_test "ls > test jambon > test2"
run_test "ls <& test"
run_test "ls .. > test ;echo tonton >> test"
run_test "ls toto > test; cat test"
run_test "ls < test; cat test"
run_test "1> ls | cat echo"

rm -f test test2 jambon toto
