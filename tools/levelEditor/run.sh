cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
cp build/level-editor .
