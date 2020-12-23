#!/bin/bash

FILE=conquest

echo "Compiling $FILE"
./compile.sh
cd build
./$FILE
