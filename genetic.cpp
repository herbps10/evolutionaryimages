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

#include "header.h"
#include "image.h"
#include "image.cpp"




int compare(Image *a, Image *b)
{
	return a->fitness < b->fitness;
}

int main(int argc, char** argv)
{
	srandom((unsigned int)time(NULL));

	Image *target = new Image();

	target->load_from_file((char*)"target.bmp");

	//target->randomize_pixels();
	//target->set_color(255, 0, 0);

	cout << "Target:" << endl;
	target->print_pixel(0, 0);
	cout << endl;

	Image *population[POPULATION_SIZE];


  Image *new_individual = new Image();
  new_individual->allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  Image *temp;

  int elitism = 10;

	//FILE *stats = fopen("population_size_test.csv", "w");
	//fprintf(stats, "population_size, fitness\n");

  // generate the population
  for(int i = 0; i < POPULATION_SIZE; i++)
  {
    cout << i << " " << population[i] << endl;
    population[i] = new Image();
    population[i]->randomize_pixels();

    population[i]->print_pixel(0, 0);

    //population[i]->calculate_fitness(*target); //this allocates a new object and then passes it by value every time
    population[i]->calculate_fitness(target); 

    cout << population[i]->fitness << endl << endl;

    //population[i]->randomize_polygons();
  }

  // Sort the population
  sort(population, population + POPULATION_SIZE, compare);
  
  for(int generation = 0; generation < GENERATIONS; generation++)
  {
    int i1 = (int)(rand_one() * (float)elitism); 
    int i2 = (int)(rand_one() * (float)elitism); 

    //population[i1]->recombine(*population[i2]);
    new_individual->recombine_pixels(population[i1], population[i2]);
    new_individual->mutate_pixels();
    new_individual->calculate_fitness(target);

    // If the new individual is better than the worst in the current population,
    // replace the worst one with the new one
    if(new_individual->fitness < population[POPULATION_SIZE - 1]->fitness)
    {
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
      cout << generation << " " << population[0]->fitness << endl;
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
