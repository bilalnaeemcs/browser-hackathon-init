#!/bin/sh


cd build;
cmake ..;
make -j 48;
cd ..;
./build/SimpleBrowser > output.log 2>&1;

