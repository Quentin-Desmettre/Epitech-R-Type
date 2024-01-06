#! /bin/bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
cp build/r-type_server .
cp build/r-type_client .
