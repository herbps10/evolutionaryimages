#!/bin/bash
# takes output file name as an optional argument
if [ `uname` == Darwin ]; then
    #COMPILER='g++-mp-4.7 -g'
    COMPILER="clang++ -O2"
    #g++ -Wall -o evolution algorithm.cpp draw_polygons.cpp -lglut -lm -lpthread -L/usr/X11R6/lib -lm -lpthread -lX11 -framework OpenGL -framework GLUT -framework Cocoa -I/usr/X11R6/include
    $COMPILER -Wall -o genetic genetic.cpp -lglut -lm -lpthread -L/usr/X11R6/lib -lm -lpthread -lX11 -framework OpenGL -framework GLUT -framework Cocoa -I/usr/X11R6/include 
    #g++ -Wall -o genetic genetic.cpp -g -L/usr/X11R6/lib -lX11 -I/usr/X11R6/include
    exit $?
else
    #g++ -Wall -o evolution algorithm.cpp draw_polygons.cpp -lglut
    g++ -Wall -o genetic genetic.cpp -g
    exit $?
fi
exit -1
