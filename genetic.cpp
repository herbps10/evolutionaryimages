// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>

#include "CImg.h"

using namespace std;
using namespace cimg_library;

float rand_one()
{
#if defined(__MACOSX__) || defined(__APPLE__)
  return (float)((double)arc4random() / (double)4294967295); // largest value is 2^32-1 (roughly double RAND_MAX)
#else
  return (float)random() / (float)RAND_MAX;
#endif
}


#include "header.h"
#include "image.h"
#include "image.c"

int compare(Image *a, Image *b)
{
	return a->fitness < b->fitness;
}

int main(int argc, char** argv)
{
	srandom((unsigned int)time(NULL));

	Image *target = new Image();

	target->load_from_file("simple_target.bmp");

	Image *population[POPULATION_SIZE];

	// Generate initial population

	for(int i = 0; i < POPULATION_SIZE; i++)
	{
		population[i] = new Image();
		population[i]->randomize_polygons();
	}

	for(int generation = 0; generation < GENERATIONS; generation++)
	{
		for(int i = 0; i < POPULATION_SIZE; i++)
		{
			population[i]->render_cimg(false, "");
			population[i]->calculate_fitness(*target);
		}

	
		sort(population, population + POPULATION_SIZE, compare);

		for(int i = 10; i < POPULATION_SIZE; i++)
		{
			int i1 = random() / RAND_MAX * 10;
			int i2 = random() / RAND_MAX * 10;
			delete population[i];
			population[i] = population[i1]->recombine(*population[i2]);
			population[i]->mutate();
		}
		
		cout << "Generation: " << generation << " " << population[0]->fitness << endl;
	}
	
	
	cout << population[0]->fitness << endl;

	population[0]->render_cimg(true, "best.bmp");	

	return 0;
}
