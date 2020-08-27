#!/bin/bash
make clean
bear --use-only --use-c++ clang++ make
run-clang-tidy -header-filter='.*' -checks='-*,modernize-*,-modernize-use-trailing-return-type,-modernize-deprecated-headers,-modernize-avoid-c-arrays,performance-*,llvm-include-order' -fix