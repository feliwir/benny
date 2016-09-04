#!/bin/bash
echo "Formatting code"
find kernel/ -name *.c -or -name *.cpp -or -name *.hpp | xargs clang-format -i