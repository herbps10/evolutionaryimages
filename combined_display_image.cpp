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
#include <fstream>
#include "CImg.h"
#include <cstdlib> 
#include <signal.h>
#include <unistd.h>

#define ISFULLSCREEN true 
#define PROJECT_PATH "/home/srk3/evolving_images/"

using namespace cimg_library;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;

CImg<unsigned char> *img = NULL;
CImg<unsigned char> *next_to_display = NULL;
CImg<unsigned char> *tmp = NULL;
CImgDisplay *main_display;

int  numtasks, rank, rc; // for mpi
int update; // buffer for status codes send via mpi
char filename[128]; // file name to be sent via mpi


void crop_for_wall(CImg<unsigned char> *image) {
	int x,y;
	x = rank%4;
	y = rank/4;
		image->crop(image->width()*x/4, image->height()*y/4,   image->width()*(x+1)/4, image->height()*(y+1)/4 );
		image->resize( CImgDisplay::screen_width() , CImgDisplay::screen_height()  );
}

CImg<unsigned char>* get_next_image_to_display() {
  CImg<unsigned char>* next = NULL;
  string token;
  ifstream myfile;

  myfile.open (PROJECT_PATH"display_control");

  myfile >> token;
  if ( token == "individual" ) {
  	cout << "indv\n";
  }
  else if (token == "wall") {
    myfile >> std::ws;
    getline (myfile, token);
	cout << "next: " << token << endl;
	next = new CImg<unsigned char>(token.c_str());
	crop_for_wall(next);
	cout << "wall\n";
  }

  myfile.close();
  return next;
}

void sig_handler(int signo) {
	if ( signo == SIGUSR1 ) {
	    printf("hi\n");
		MPI_Bcast(&update, 1, MPI_INT, 0, MPI_COMM_WORLD );
	    next_to_display = get_next_image_to_display();
	    MPI_Bcast(&update, 1, MPI_CHAR, 0, MPI_COMM_WORLD );
	    main_display=new CImgDisplay(*next_to_display, "", 3, ISFULLSCREEN, false);
	} else if ( signo == SIGTERM || signo == SIGINT) {
		MPI_Finalize();
		exit(0);
    } else
		fprintf(stderr, "signal %d was not handled\n", signo);
}

void display_one(CImg<unsigned char> *image) {
	//crop the image
	int x,y;
	x = rank%4;
	y = rank/4;
		image->crop(image->width()*x/4, image->height()*y/4,   image->width()*(x+1)/4, image->height()*(y+1)/4 );
		image->resize( CImgDisplay::screen_width() , CImgDisplay::screen_height()  );

	//main_display->assign(img,"",3,ISFULLSCREEN,false);
	main_display=new CImgDisplay(*img, "", 3, ISFULLSCREEN, false);
	
}

void display_individual(CImg<unsigned char> *image) {
	image->resize( CImgDisplay::screen_width() , CImgDisplay::screen_height()  );
	main_display=new CImgDisplay(*img, "", 3, ISFULLSCREEN, false);
}

int main(int argc, char * argv[]) {
	
	//initialize mpi
	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if( signal(SIGUSR1, sig_handler) == SIG_ERR) {
		fprintf(stderr,"Cannot catch SIGUSR1\n");
	}
	//if( signal(SIGINT, sig_handler) == SIG_ERR) {
	//	fprintf(stderr,"Cannot catch SIGINT\n");
	//}
	//if( signal(SIGTERM, sig_handler) == SIG_ERR) {
	//	fprintf(stderr,"Cannot catch SIGTERM\n");
	//}
	
	if (argc > 1) {
		img = new CImg<unsigned char>(argv[1]);
	}
	else{
		img = new CImg<unsigned char>(16,16,1,3,50); // black image      
	}

	display_individual(img);
	cout << rank << endl;

	if(rank==0) {
		MPI_Bcast(&update, 1, MPI_INT, 0, MPI_COMM_WORLD );
		next_to_display = get_next_image_to_display();
		MPI_Bcast(&update, 1, MPI_CHAR, 0, MPI_COMM_WORLD );
		main_display=new CImgDisplay(*next_to_display, "", 3, ISFULLSCREEN, false);
		sleep(10000000);
	}
	else {
		while(true) {
			MPI_Bcast(&update, 1, MPI_INT, 0, MPI_COMM_WORLD );
			next_to_display = get_next_image_to_display();
			MPI_Bcast(&update, 1, MPI_CHAR, 0, MPI_COMM_WORLD );
			main_display=new CImgDisplay(*next_to_display, "", 3, ISFULLSCREEN, false);
			cout << "rank: " << rank << endl;
		}
	}

	MPI_Finalize();
	exit(0);
	
}


