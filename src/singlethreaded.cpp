// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>

#include "CImg.h"

using namespace std;
using namespace cimg_library;

#include "header.h"
#include "functions.h"

#include "image.h"
#include "image.cpp"

#include "functions.cpp"

#include "population.h"
#include "population.cpp"

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
    cout << "Loading default image targets/target.bmp" << endl;
	  target->load_from_file((char *)"targets/target.bmp");
  }

  Population *population = new Population(target);

  // Not really generations anymore since each new individual is added immediately
  for(int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    population->iterate();

    if((iteration & 0x1ff) == 0)
    {
      cout << iteration << " " << population->get(0)->fitness << " " << population->get(POPULATION_SIZE - 1)->fitness - population->get(0)->fitness << endl;

      char filename[100];
      snprintf(filename, 100, "output/iteration-%i.gif", iteration);

      population->save_individual(0, filename);
    }
  }

  return 0;
}
