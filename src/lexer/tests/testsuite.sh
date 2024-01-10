#!/bin/sh

function check
{
  var="$*"
  DIFF=$(diff -u <( ./lexer ${var} 2>/dev/null) <( find ${var} 2>/dev/null))
  if [ "$DIFF" != "" ]; then
    printf "\e[31m[ FAIL ]\e[39m  ";
    printf "%b\n" "$var"
    printf "%s\n" $DIFF;
  else
    printf "\e[32m[ OK ]\e[39m\t  "
    printf "%b\n" "$var"
  fi
}

check
check "../"
check "src" "include/"
check "src"
check "src" "!" "-name src"
check "src" "-name" "src"
check "-print" "-print"
check "-type" "d" "-name" "test*"
check "-type" "f" "-a" "-name" "memory*"
#check -L
check "-q"
check "../" "-type" "b"
check "../" "-type" "c"
check "../" "-type" "d"
check "../" "-type" "f"
check "../" "-type" "l"
check "../" "-type" "p"
check "../" "-type" "s"
check "-print" "-o" "-print"
check "-print" "-a" "-print"
check "-type" "d" "!" "-name" "include"
check "-type" "f" "-name" "memory*" "-o" "-type" "d" "-print"
check "src/include/" "-print" "-a" "-user" "elena.villepreux"
check "src/" "-type" "f" "-name" "*.o"
check "(" "-name" "mem*" "-o" "!" "-name" "eval*" ")"
check "src" "-exec" "pwd" ";"
check "tests" "-exec" "echo" "{}" ";"
check "src/include/" "-name" "mem*" "-exec" "echo" "{}" ";"
#check "src" "-execdir" "echo" "{}" ";"
check "!" "-name" "tests"
check "!" "(" "-name" "tests" "-o" "-name" "src" ")"
check "(" "-name" "*.c" "-o" "-name" "memory*" ")"
check "(" ")"
check "src/include/" "-perm" "-644"
check "src/include/" "-perm" "/644"
check "src/include/" "-perm" "644"
check "src/include/" "-perm" "238"
check "src/include/" "-perm" "21"
check "src/include/" "!" "-type" "d"
check "(" "-type" "f" "-name" "memory*" ")" "-o" "(" "-name" "memory*" ")"
check "-name" "me*" "-o"  "!" "(" "-type" "d" "-a" "-name" "include" ")" "-o" "-name" "memory.o"
check "src/include/" "-newer" "src/include/memory.h"
check "src/include/" "-newer" "src/include/memory.h" "!" "-name" "*.h"
#check "tests" "-name" "test1" "-delete" "-delete"
#check "tests" "-name" "direct1" "-delete"


./lexer smt > test.out
echo res > test.ref
DIFF test.out test.ref


