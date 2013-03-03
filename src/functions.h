#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include "image.h"

inline float rand_one();
inline float rand_range(float min, float max);
bool pointInTriangle(polygon *triangle, float x, float y);
int compare(Image *a, Image *b);
int weighted_random_parent(Image **population, float fitness_sum);

#endif
