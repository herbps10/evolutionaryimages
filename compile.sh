#!/bin/bash
# takes output file name as an optional argument
if [ $1 ]; then
    g++ -Wall -o "$1" algorithm.cpp draw_polygons.cpp -lglut
    exit $?
else
    g++ -Wall -o evolution algorithm.cpp draw_polygons.cpp -lglut
    exit $?
fi
exit -1
