inline float rand_one()
{
#if defined(__MACOSX__) || defined(__APPLE__)
  return (float)((double)arc4random() / (double)4294967295); // largest value is 2^32-1 (roughly double RAND_MAX)
#else
  return (float)random() / (float)RAND_MAX;
#endif
}

inline float rand_range(float min, float max)
{
#if defined(__MACOSX__) || defined(__APPLE__)
  return (float)arc4random() / (float)4294967295 * (max - min) + min; // largest value is 2^32-1 (roughly double RAND_MAX)
#else
  return (float)random() / (float)RAND_MAX * (max - min) + min;
#endif
}

bool pointInTriangle(polygon *triangle, float x, float y)
{
  float lambda1, lambda2, lambda3;

  // reduce the amount of expensive floating point math
  float denominator = (triangle->points[1].y - triangle->points[2].y) * (triangle->points[0].x - triangle->points[2].x) + (triangle->points[2].x - triangle->points[1].x) * (triangle->points[0].y - triangle->points[2].y);
  float x_x3 = x - triangle->points[2].x;
  float y_y3 = y - triangle->points[2].y;

  //Convert to barycentric coordinate system
  lambda1 = ((triangle->points[1].y - triangle->points[2].y)*(x_x3) + (triangle->points[2].x - triangle->points[1].x)*(y_y3)) / denominator;
  if(lambda1 < 0.0f || lambda1 > 1.0f) return false;

  lambda2 = ((triangle->points[2].y - triangle->points[0].y)*(x_x3) + (triangle->points[0].x - triangle->points[2].x)*(y_y3)) / denominator;
  if(lambda2 < 0.0f || lambda2 > 1.0f) return false;

  lambda3 = 1.0f - lambda2 - lambda1;
  if(lambda3 < 0.0f || lambda3 > 1.0f) return false;

  return true;
}


int compare(Image *a, Image *b)
{
	return a->fitness < b->fitness;
}


//
// This method isn't finished -- right now it 
// gives higher weight to the individuals with higher fitness.
// this is backwards, since lower fitness is better. 
// we need to figure out how to do an inverse weighted random
// parent selector.
//
int weighted_random_parent(Image **population, float fitness_sum)
{
  float t = rand_one() * fitness_sum;

  for(int i = 0; i < POPULATION_SIZE; i++)
  {

    if(t < population[i]->fitness)
    {
      return i;
    }

    t -= population[i]->fitness;
  }

  return POPULATION_SIZE - 1;
}
