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

	//target->load_from_file("target.bmp");

	target->randomize_pixels();
	//target->set_color(255, 0, 0);

	cout << "Target:" << endl;
	target->print_pixel(0, 0);
	cout << endl;

	Image *population[1000];

	int elitism = 10;

	FILE *stats = fopen("population_size_test.csv", "w");
	fprintf(stats, "population_size, fitness\n");

	// Generate initial population
	for(POPULATION_SIZE = 50; POPULATION_SIZE <= 1000; POPULATION_SIZE += 50)
	{
		for(int rep = 0; rep < 10; rep++)
		{
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
			
			for(int generation = 0; generation < GENERATIONS; generation++)
			{
				for(int i = 0; i < POPULATION_SIZE; i++)
				{
					//population[i]->render_cimg(false, "");
					//population[i]->calculate_fitness(*target);
				    population[i]->calculate_fitness(target); 
				}

			
				sort(population, population + POPULATION_SIZE, compare);

				for(int i = elitism; i < POPULATION_SIZE; i++)
				{
					int i1 = random() / RAND_MAX * elitism;
					int i2 = random() / RAND_MAX * elitism;
                    //fprintf(stderr, "    Herb: i1 is %d and i2 is %d\n", i1, i2);
                    
                    // did you mean?
                    i1 = (int)(rand_one() * (float)elitism);
                    i2 = (int)(rand_one() * (float)elitism);

					//population[i1]->recombine(*population[i2]);
					population[i]->recombine_pixels(population[i1], population[i2]);
					population[i]->mutate_pixels();
					//population[i]->mutate();
				}
				
				if(generation % 10 == 0)
				{
					cout << "Generation: " << generation << " " << population[0]->fitness << endl;
				}
			}

			fprintf(stats, "%i, %f\n", POPULATION_SIZE, population[0]->fitness);
			fflush(stats);

            //reclaim memory
            for(int i =0; i < POPULATION_SIZE; i++) {
                delete(population[i]);
            }
		}
	}

	fclose(stats);
	
	cout << population[0]->fitness << endl;

	population[0]->save((char*)"best.bmp");
	target->save((char*)"target.bmp");

	return 0;
}
