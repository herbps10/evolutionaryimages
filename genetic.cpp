// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>

#include "CImg.h"

using namespace std;
using namespace cimg_library;

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
  return (((float)random() / (float)RAND_MAX) * (max - min) + min);
}

#include "header.h"

bool pointInTriangle(polygon *triangle, float x, float y)
{
  float y1 = triangle->points[0].y;
  float y2 = triangle->points[1].y;
  float y3 = triangle->points[2].y;

  float x1 = triangle->points[0].x;
  float x2 = triangle->points[1].x;
  float x3 = triangle->points[2].x;

  float lambda[3];

  // reduce the amount of expensive floating point math
  float denominator = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);

  //Convert to barycentric coordinate system
  lambda[0] = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / denominator;

  lambda[1] = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / denominator;

  lambda[2] = 1.0f - lambda[0] - lambda[1];

  // If lambda 1, 2, and 3 are between 0 and 1, then the point x,y is in the triangle

  for(int i = 0; i < 3; i++)
  {
    if(lambda[i] < 0.0f || lambda[i] > 1.0f)
    {
      return false;
    }
  }

  return true;
}


#include "image.h"
#include "image.cpp"

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

int main(int argc, char** argv)
{
	srandom((unsigned int)time(NULL));

	Image *target = new Image();

	target->load_from_file("target.bmp");
  //target->allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  //target->randomize_polygons();
  //target->render_scanline();
	//target->set_color(0, 255, 0);


	Image *population[POPULATION_SIZE];


  Image *new_individual = new Image();
  new_individual->allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  Image *temp;

  int elitism = 200;
  float fitness_sum = 0;

  // generate the population
  for(int i = 0; i < POPULATION_SIZE; i++)
  {
    population[i] = new Image();
    population[i]->allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    population[i]->randomize_polygons();
    population[i]->render_scanline();
    population[i]->calculate_fitness(target); 

    fitness_sum += population[i]->fitness;
  }

  // Sort the population
  sort(population, population + POPULATION_SIZE, compare);

  for(int generation = 0; generation < GENERATIONS; generation++)
  {
    //int i1 = weighted_random_parent(population, fitness_sum);
    //int i2 = weighted_random_parent(population, fitness_sum);
    int i1 = rand_one() * elitism;
    int i2 = rand_one() * elitism;

    new_individual->recombine(population[i1], population[i2]);
    new_individual->mutate();

    new_individual->render_scanline();
    new_individual->calculate_fitness(target);

    //cout << population[POPULATION_SIZE - 1]->fitness << " "  << new_individual->fitness << endl;

    // If the new individual is better than the worst in the current population,
    // replace the worst one with the new one
    if(new_individual->fitness < population[POPULATION_SIZE - 1]->fitness)
    {
      // Update the sum of fitnesses in the population
      fitness_sum -= population[POPULATION_SIZE - 1]->fitness;
      fitness_sum += new_individual->fitness;

      // Replace the worst individual with the new one
      temp = new_individual;
      new_individual = population[POPULATION_SIZE - 1];
      population[POPULATION_SIZE - 1] = temp;

      // Bubble the new individual up until it is in the right spot
      int index = POPULATION_SIZE - 1;
      while(index > 0 && population[index - 1]->fitness > population[index]->fitness)
      {
        temp = population[index - 1];
        population[index - 1] = population[index];
        population[index] = temp;

        index--;
      }
    }

    if(generation % 1000 == 0)
    {
      cout << generation << " " << population[0]->fitness << " " << population[POPULATION_SIZE - 1]->fitness - population[0]->fitness << endl;

      char filename[100];
      snprintf(filename, 100, "output/generation-%i.bmp", generation);

      population[0]->save(filename);
    }
  }
	
	cout << population[0]->fitness << endl;

	population[0]->save((char*)"best.bmp");
	target->save((char*)"target.bmp");

  //fprintf(stats, "%i, %f\n", POPULATION_SIZE, population[0]->fitness);
  //fflush(stats);
	//fclose(stats);

  //reclaim memory
  for(int i = 0; i < POPULATION_SIZE; i++) {
      delete(population[i]);
  }
	return 0;
}
