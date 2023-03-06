#!/bin/bash

clear
ls="ls -lh *.o *.a simple_test_alloc"
echo -e "\033[1mBefore compiling:\033[0m"
${ls}
echo; echo
echo -e "\033[1mClean:\033[0m"
make clean
echo; echo
echo -e "\033[1mCompile:\033[0m"
make
echo; echo
echo -e "\033[1mAfter compiling:\033[0m"
${ls}
