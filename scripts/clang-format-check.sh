#!/bin/bash

# This script is used to check if the code is formatted correctly.

# File extensions to check
FILE_EXT="cpp|cc|h|hpp|cxx|c++|c"

# Run clang-format
clang-format -dry-run --style=file:./.clang-format -Werror $(git ls-files | grep -E "\.($FILE_EXT)$") 2> error.log

# print badly formatted file names, if any
if [ -s error.log ]; then
    cat error.log | grep -E "\.($FILE_EXT):[0-9]+:[0-9]+: error:" | awk -F ":" '{print $1}' | sort | uniq > error_files.log
    # annotate the files with error, for github actions
    (
        while read file; do
            echo "::error file=$file::clang-format check failed"
        done
    ) < error_files.log
    rm error_files.log
    rm error.log
    exit 1
fi
rm error.log
