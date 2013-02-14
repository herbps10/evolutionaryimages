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

  if(argc > 1)
  {
    target->load_from_file(argv[1]);
  }
  else
  {
    cout << "Loading default image target.bmp" << endl;
	  target->load_from_file((char *)"target.bmp");
  }
  //target->allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  //target->randomize_polygons();
  //target->render_scanline();
	//target->set_color(0, 255, 0);


	Image *population[POPULATION_SIZE];


  Image *new_individual = new Image();
  new_individual->allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  Image *temp;

  int elitism = 20;
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

  // Not really generations anymore since each new individual is added immediately
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

    if(generation % 10000 == 0)
    {
      cout << generation << " " << population[0]->fitness << " " << population[POPULATION_SIZE - 1]->fitness - population[0]->fitness << endl;

      char filename[100];
      snprintf(filename, 100, "output/generation-%i.bmp", generation);

      population[0]->save(filename);
    }
  }
	
	cout << population[0]->fitness << endl;

	//population[0]->save((char*)"best.bmp");
	//target->save((char*)"target.bmp");

  //fprintf(stats, "%i, %f\n", POPULATION_SIZE, population[0]->fitness);
  //fflush(stats);
	//fclose(stats);

  //reclaim memory
  for(int i = 0; i < POPULATION_SIZE; i++) {
      delete(population[i]);
  }
	return 0;
}
