#!/bin/sh

#Execute the scripts which have in their name:
# - the category
# - the sort of input
# - both
execute_selected_tests()
{
    first="$1"
    second="$2"

    for script in tests/*.sh; do
        if [ -x "$script" ] && [ -n "$(echo "$script" | grep "$first")" ] && [ -n "$(echo "$script" | grep "$second")" ]; then
                echo "Execution de $script"
                ./"$script"
        fi
    done
}

execute_all_tests()
{
    for script in tests/testsuite*.sh; do
        if [ -x "$script" ]; then
            ./"$script"
        fi
    done
}

if [ "$#" -eq 0 ]; then
    execute_all_tests

elif [ "$#" -eq 1 ]; then
    execute_selected_tests "$1"

elif [ "$#" -eq 2 ]; then
    execute_selected_tests "$1" "$2"

else
    exit 1
fi
