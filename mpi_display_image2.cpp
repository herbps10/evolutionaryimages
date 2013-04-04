/*
 *  display 16 images
 *  
 *
 *  Created by Stephen Kowalewski on 4/23/11.
 *
 */

// g++ -o imTest image.cpp -O2 -lm -lpthread -L/usr/X11R6/lib -lX11

#include <mpi.h>
#include <iostream>
#include "CImg.h"
#include <cstdlib> 

#define ISFULLSCREEN true 

using namespace cimg_library;
using std::cin;
using std::cout;
using std::endl;


int main(int argc, char * argv[]) {
	
	int  numtasks, rank, rc; // for mpi

	//initialize mpi
	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
		CImg<unsigned char> img;
		if (argc > 1) {
			img.load(argv[1]);
		}
		else{
			img.load("p.jpg");       
		}
		
		//crop the image
		int x,y;
		x = rank%4;
		y = rank/4;
			img.crop(img.width()*x/4, img.height()*y/4,   img.width()*(x+1)/4, img.height()*(y+1)/4 );
			img.resize( CImgDisplay::screen_width() , CImgDisplay::screen_height()  );

		CImgDisplay main_display(img,"",3,ISFULLSCREEN,false);
		
		while(true)
			sleep(100000);

		MPI_Finalize();
		exit(0);
		
}


