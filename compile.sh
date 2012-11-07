#!/bin/bash
if [ $1 ]; then
    if [ "`uname`" == "Darwin" ]; then
        clang++ -o "$1" "$1.cpp" -framework GLUT -framework OpenGL -framework Cocoa
    else
        g++ -o "$1" "$1.cpp" -lglut
    fi
else
    echo Please specify an input file
    exit 1
fi
exit 0
