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

	//target->load_from_file("target.bmp");

	int elitism = 10;

	target->randomize_pixels();
	//target->set_color(255, 0, 0);

	cout << "Target:" << endl;
	target->print_pixel(0, 0);
	cout << endl;

	Image *population[POPULATION_SIZE];

	// Generate initial population
	for(int i = 0; i < POPULATION_SIZE; i++)
	{
		cout << i << " " << population[i] << endl;
		population[i] = new Image();
		population[i]->randomize_pixels();

		population[i]->print_pixel(0, 0);

		population[i]->calculate_fitness(*target);

		cout << population[i]->fitness << endl << endl;

		//population[i]->randomize_polygons();
	}
	
	for(int generation = 0; generation < GENERATIONS; generation++)
	{
		for(int i = 0; i < POPULATION_SIZE; i++)
		{
			//population[i]->render_cimg(false, "");
			population[i]->calculate_fitness(*target);
		}

	
		sort(population, population + POPULATION_SIZE, compare);

		for(int i = elitism; i < POPULATION_SIZE; i++)
		{
			int i1 = random() / RAND_MAX * elitism;
			int i2 = random() / RAND_MAX * elitism;

			//population[i1]->recombine(*population[i2]);
			population[i]->recombine_pixels(*population[i1], *population[i2]);
			population[i]->mutate_pixels();
			//population[i]->mutate();
		}
		
		if(generation % 10 == 0)
		{
			cout << "Generation: " << generation << " " << population[0]->fitness << endl;
		}
	}

	cout << population[0]->fitness << endl;

	population[0]->save("best.bmp");
	target->save("target.bmp");

	return 0;
}
