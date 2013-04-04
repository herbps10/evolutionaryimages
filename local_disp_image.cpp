/*
 *  display 16 images
 *  
 *
 *  Created by Stephen Kowalewski on 4/23/11.
 *
 */

// g++ -o imTest image.cpp -O2 -lm -lpthread -L/usr/X11R6/lib -lX11

#include <iostream>
#include "CImg.h"
#include <cstdlib> 

#define ISFULLSCREEN true 

using namespace cimg_library;
using std::cin;
using std::cout;
using std::endl;


int main(int argc, char * argv[]) {

	CImg<unsigned char> img;
	if (argc > 1) {
		img.load(argv[1]);
	}
	else{
		img.load("p.jpg");       
	}
	
	img.resize( CImgDisplay::screen_width() , CImgDisplay::screen_height() );

	CImgDisplay main_display(img,"",3,ISFULLSCREEN,false);
	
	while(true)
		sleep(100000);

	exit(0);
}

