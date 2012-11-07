#!/bin/bash
if [ $1 ]; then
    clang -o $1 $1.c -framework GLUT -framework OpenGL -framework Cocoa
else
    echo Please specify an input file
    exit 1
fi
exit 0
