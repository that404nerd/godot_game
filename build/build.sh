#!/bin/bash

cmake --build FPSProject-build -j$(nproc)
cmake --install FPSProject-build
