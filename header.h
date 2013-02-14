#ifndef OUR_HEADER
#define OUR_HEADER

#if defined(__MACOSX__) || defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "CImg.h"

#define GENERATIONS 2000000
#define MUTATION_PR 0.5
#define MUTATION_SPREAD 5

#define DEFAULT_WIDTH 100
#define DEFAULT_HEIGHT 100
#define MAX_POINTS 3 //maximum points per polygon
#define MAX_POLYGONS 1000

#define POPULATION_SIZE 1000

typedef struct vertex3f {
    float x;
    float y;
    float z; // not particularly relevant for our purposes, but must be there
} vertex3f;

typedef struct color4f {
    float r;
    float g;
    float b;
    float a; // alpha
} color4f;

typedef struct polygon {
    int num_p; // number of points (cannot exceed MAX_POINTS)
    color4f color;
    vertex3f points[MAX_POINTS];
} polygon;

//function declarations from draw_polygons.cpp
//void timer_callback(int value);
void display(void);
void init(void);

//function declarations for algorithm.cpp
void beginGeneticAlgorithm();


#endif
