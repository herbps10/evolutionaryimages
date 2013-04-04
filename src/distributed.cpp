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
  int err, rank, num_nodes;

  // Initialize
  err = MPI_Init(&argc, &argv);

  if(err != MPI_SUCCESS)
  {
    cout << "Error initializing MPI" << endl;
    exit(1);
  }

  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get our rank
  MPI_Comm_size(MPI_COMM_WORLD, &num_nodes); // Get the number of nodes


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

  Population *population = new Population(target);

  float *send_buffer; // Used for sending images

  // Not really generations anymore since each new individual is added immediately
  for(int iteration = 0; iteration < ITERATIONS; iteration++)
  {
    // Test to see if we have recieved a new individual yet
    MPI_Test(&request, &flag, &status);

    // If the recieve is complete, then we have an individual from another host to deal with
    if(flag == 1)
    {
      cout << "Rank " << 1 << " Recieved individual with buffer[0]=" << recieve_buffer[0] << endl;
      
      population->add_from_buffer(recieve_buffer);

      population->test_new_individual();
      
      // Restart the recieve
      MPI_Irecv(recieve_buffer, target->pack_size(), MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
    }

    population->iterate();

    if((iteration % 5000) == 0)
    {
      // Send best individual to a random host
      send_buffer = population->get(0)->pack();

      int recv_node = rand() % num_nodes;
      //int recv_node = 1;

      MPI_Request request;
      MPI_Isend(send_buffer, target->pack_size(), MPI_FLOAT, recv_node, 0, MPI_COMM_WORLD, &request);

      cout << "Send best individual to " << recv_node << " send buffer " << send_buffer[0] << endl;

      cout << "Rank " << rank << " iteration " << iteration << ": best fitness of " << population->get(0)->fitness << " with a fitness range of " << population->get(POPULATION_SIZE - 1)->fitness - population->get(0)->fitness << endl;

      char filename[100];
      snprintf(filename, 100, "output/%i/iteration-%i.gif", rank, iteration);
      population->save_individual(0, filename);

      char command[200];
      snprintf(command, 200, "cp /home/herb/git/evolutionaryimages/output/%i/iteration-%i.gif /home/herb/git/evolutionaryimages/output/%i/recent.gif", rank, iteration, rank );

      system(command);
    }
  }

  MPI_Finalize();

  return 0;
}
