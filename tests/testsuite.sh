#!/bin/sh

echo -e "\e[34m================== INPUT AS STRING =================\e[0m"
tests/./testsuite_string.sh
echo -e "\e[34m================== INPUT AS FILE ===================\e[0m"
tests/./testsuite_file.sh
echo -e "\e[34m================== INPUT AS PIPE ===================\e[0m"
tests/./testsuite_pipe.sh
echo -e "\e[34m================== INPUT AS REDIR ==================\e[0m"
tests/./testsuite_redir.sh
echo -e "\e[34m====================================================\e[0m"
