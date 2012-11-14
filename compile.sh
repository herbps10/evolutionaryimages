#!/bin/bash
if [ $1 ]; then
    if [ "`uname`" == "Darwin" ]; then
        g++ -o "$1" draw_polygons.cpp algorithm.cpp  -framework GLUT -framework OpenGL -framework Cocoa -O2 -lm -lpthread -L/System/Library/Frameworks/Tk.framework/Versions/8.5/Headers/ -lX11
    else
        g++ -o "$1" "$1.cpp" -lglut
    fi
else
    $0 geometric_art 
fi
exit 0
