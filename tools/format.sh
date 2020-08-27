#!/bin/bash
echo "Formatting code"
find kernel/ -name *.c -or -name *.cpp -or -name *.hpp -or -name *.cppm | xargs clang-format -i