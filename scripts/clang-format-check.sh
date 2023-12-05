#!/bin/bash

# This script is used to check if the code is formatted correctly.

# File extensions to check
FILE_EXT="cpp|cc|h|hpp|cxx|c++|c"

# Run clang-format
clang-format -dry-run --style=file:./.clang-format -Werror $(git ls-files | grep -E "\.($FILE_EXT)$") 2> error.log

# print badly formatted file names
# line example: src/utils/Matrix.cpp:285:17: error: code should be clang-formatted [-Wclang-format-violations]
# expected: src/utils/Matrix.cpp

if [ -s error.log ]; then
    echo "The following files are not clang-formatted:"
    cat error.log | grep -E "\.($FILE_EXT):[0-9]+:[0-9]+: error:" | awk -F ":" '{print $1}' | sort | uniq
    exit 1
fi
