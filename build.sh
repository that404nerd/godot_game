#!/bin/bash

cd build/
cmake --build FPSProject-build -j8
cmake --install FPSProject-build
cd .. # We are in the root of the game project
rm compile_commands.json
ln -s build/FPSProject-build/compile_commands.json .
