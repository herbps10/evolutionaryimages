#!/bin/bash
# takes output file name as an optional argument
if [ `uname` == Darwin ]; then
    clang++ -Wall -o evolution algorithm.cpp draw_polygons.cpp -lglut -lm -lpthread -L/usr/X11R6/lib -lm -lpthread -lX11 -framework OpenGL -framework GLUT -framework Cocoa -I/usr/X11R6/include
    exit $?
else
    g++ -Wall -o evolution algorithm.cpp draw_polygons.cpp -lglut
    exit $?
fi
exit -1
