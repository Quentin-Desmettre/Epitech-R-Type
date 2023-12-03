#!/bin/bash

# This script is used to auto-format the code.

# File extensions to check
FILE_EXT="cpp|cc|h|hpp|cxx|c++|c"

# Run clang-format
clang-format -i --style=file:./.clang-format $(git ls-files | grep -E "\.($FILE_EXT)$")
