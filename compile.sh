#!/bin/bash
# takes output file name as an optional argument
if [ `uname` == Darwin ]; then
    #COMPILER='g++-mp-4.7 -g'
    COMPILER="clang++ -O2"
    #g++ -Wall -o evolution algorithm.cpp draw_polygons.cpp -lglut -lm -lpthread -L/usr/X11R6/lib -lm -lpthread -lX11 -framework OpenGL -framework GLUT -framework Cocoa -I/usr/X11R6/include
    $COMPILER -Wall -o bin/singlethreaded src/singlethreaded.cpp -lglut -lm -lpthread -L/usr/X11R6/lib -lm -lpthread -lX11 -framework OpenGL -framework GLUT -framework Cocoa -I/usr/X11R6/include 
    #g++ -Wall -o genetic genetic.cpp -g -L/usr/X11R6/lib -lX11 -I/usr/X11R6/include
    exit $?
else
    #g++ -Wall -o bin/singlethreaded src/singlethreaded.cpp -lX11 -lpthread -lglut -g
    mpiCC -Wall -o bin/distributed src/distributed.cpp -lX11 -lpthread -lglut -g
    exit $?
fi
exit -1
