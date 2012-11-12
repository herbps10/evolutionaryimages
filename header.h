#define DEFAULT_WIDTH 500
#define DEFAULT_HEIGHT 500
#define MAX_POINTS 6 //maximum points per polygon
#define MAX_POLYGONS 5

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
