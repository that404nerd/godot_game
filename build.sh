#!/bin/bash

cd build/
cmake --build FPSProject-build -j8
cmake --install FPSProject-build
cd ..
ln -s build/FPSProject-build/compile_commands.json .