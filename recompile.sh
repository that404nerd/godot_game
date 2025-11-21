#!/bin/bash

# Assuming that we already have the build folder

cd build/
cmake -B FPSProject-build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_INSTALL_PREFIX=FPSProject-install FPSProject ..
cd ..