#!/bin/bash
#
# Compiles genetic.cpp which does not depend on OpenGL

g++ -Wall -o genetic genetic.cpp -lglut -g
