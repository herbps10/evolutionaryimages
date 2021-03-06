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
  int err, rank, n;

  // Initialize
  err = MPI_Init(&argc, &argv);

  if(err != MPI_SUCCESS)
  {
    cout << "Error initializing MPI" << endl;
    exit(1);
  }

  // Get our rank
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n);

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


  // ---------------------
  // Setup the MPI recieve
  // ---------------------

  float *recieve_buffer = (float *)malloc(sizeof(float) * target->pack_size());

  MPI_Request request;
  MPI_Irecv(recieve_buffer, target->pack_size(), MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
  
  MPI_Status status;
  int flag = 0;

  // ---------------------------------
  // Setup & run the genetic algorithm
  // ---------------------------------
  char filename[100];
  char command[200];

  Population *population = new Population(target);

  float *send_buffer = NULL; // Used for sending images

  // Not really generations anymore since each new individual is added immediately
  for(int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    // Test to see if we have recieved a new individual yet
    MPI_Test(&request, &flag, &status);

    // If the recieve is complete, then we have an individual from another host to deal with
    if(flag == 1)
    {
      cout << "iteration " << iteration << ": Rank " << rank << " Recieved individual with buffer[0]=" << recieve_buffer[0] << endl;
      
      population->add_from_buffer(recieve_buffer);

      population->test_new_individual();
      
      // Restart the recieve
      MPI_Irecv(recieve_buffer, target->pack_size(), MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
    }
	
    // *** important part
    population->iterate();

    if((iteration % 5000) == 0)
    {
      // Send best individual to a random host
	  free(send_buffer);
      send_buffer = population->get(0)->pack();

      int recv_node = rand() % n;

      if ( recv_node != rank ) {
        MPI_Request request;
        MPI_Isend(send_buffer, target->pack_size(), MPI_FLOAT, recv_node, 0, MPI_COMM_WORLD, &request);
      }

      cout << "Send best individual to " << recv_node << " send buffer " << send_buffer[0] << endl;

      cout << "Rank " << rank << " iteration " << iteration << ": best fitness of " << population->get(0)->fitness << " with a fitness range of " << population->get(POPULATION_SIZE - 1)->fitness - population->get(0)->fitness << endl;

      snprintf(filename, 100, "output/%i-iteration-%i.jpg", rank, iteration);
      cout << "Saving to " << filename << endl;
      population->save_individual(0, filename);
    }
  }

  MPI_Finalize();

  return 0;
}
