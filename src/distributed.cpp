// Herb Susmann and Stephen Kowalewski

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <mpi.h>

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
  // Set the seed
  srandom((unsigned int)time(NULL));

  // -------------
  // Intialize MPI
  // -------------
  int err, rank;

  // Initialize
  err = MPI_Init(&argc, &argv);

  if(err != MPI_SUCCESS)
  {
    cout << "Error initializing MPI" << endl;
    exit(1);
  }

  // Get our rank
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  cout << "Rank " << rank << " Initialized." << endl;


  
  // ------------------------------
  // Read in command line arguments
  // ------------------------------

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



  // ---------------------------------
  // Setup & run the genetic algorithm
  // ---------------------------------

  Population *population = new Population(target);

  // Not really generations anymore since each new individual is added immediately
  for(int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    population->iterate();

    if((iteration & 0x1ff) == 0)
    {
      cout << "Rank " << rank << " iteration " << iteration << ": best fitness of " << population->get(0)->fitness << " with a fitness range of " << population->get(POPULATION_SIZE - 1)->fitness - population->get(0)->fitness << endl;

      char filename[100];
      snprintf(filename, 100, "output/%i/iteration-%i.gif", rank, iteration);

      population->save_individual(0, filename);
    }
  }

  MPI_Finalize();

  return 0;
}
