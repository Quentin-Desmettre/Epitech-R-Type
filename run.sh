cmake -B build
cmake --build build -DCMAKE_BUILD_TYPE=Debug
cp build/r-type_server .
cp build/r-type_client .
