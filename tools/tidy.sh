#!/bin/bash
make clean
bear make
run-clang-tidy -header-filter='.*' -checks='-*,modernize-*'