#!/bin/bash
make clean
bear --include kernel --use-only --use-c++ clang++ make
run-clang-tidy -header-filter='.*' -checks='-*,modernize-*,performance-*' -fix