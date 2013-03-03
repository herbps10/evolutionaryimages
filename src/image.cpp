int Image::width()
{
  return this->image_width;
}

int Image::height() {
  return this->image_height;
}

// assumes image_buffer and buffer are the same size
double Image::sumOfError(float *buffer, int height, int width)
{
    int i;
    double error = 0.0;

    if( height != DEFAULT_HEIGHT || width != DEFAULT_WIDTH ) {
        fprintf(stderr, "sumOfError: buffers are not the same size.\n");
        return -1.0;
    }

    for(i = 0; i < DEFAULT_HEIGHT * DEFAULT_WIDTH * 3; i++) {
        error += abs(buffer[i] - image_buffer[i]);
    }

    //error = error / (3 * 255 * DEFAULT_HEIGHT * DEFAULT_WIDTH);

    
    return error;
}


// assumes image_buffer and buffer are the same size
double Image::sumOfSquaresError(float *buffer, int height, int width)
{
    int i;
    double error = 0.0, diff;

    if( height != DEFAULT_HEIGHT || width != DEFAULT_WIDTH ) {
        fprintf(stderr, "sumOfSquaresError: buffers are not the same size.\n");
        return -1.0;
    }

    for(i = 0; i < DEFAULT_HEIGHT * DEFAULT_WIDTH * 3; i += 1) {
        diff = image_buffer[i] - buffer[i];

        // Moved normalization up here so that error size doesn't overflow
        error += diff*diff;
    }

    //error = error / (3 * 255 * DEFAULT_HEIGHT * DEFAULT_WIDTH);

    return error;
}


Image::Image()
{
    image_buffer = NULL;
    polygon_buffer = NULL;
}

Image::~Image() {
    free(image_buffer);
    free(polygon_buffer);
}

void Image::allocate_polygons()
{
  // Allocate space for the polygon_buffer
  // NOTE: image_buffer and polygon_buffer may not be the same size. Either we'll have to adjust that here or we'll have to write a fitness function that is aware of this
  // do not change the size of this buffer without making appropriate changes in display()
  polygon_buffer = (float*)malloc(sizeof(float) * DEFAULT_WIDTH * DEFAULT_HEIGHT * 3);
  if( polygon_buffer == NULL )
    fprintf(stderr, "error malloc-ing polygon_buffer\n");
}

void Image::allocate_image_buffer(int buffer_width, int buffer_height)
{
  this->image_width = buffer_width;
  this->image_height = buffer_height;

  if(image_buffer == NULL)
  {
    // Allocate space to the target image buffer
    image_buffer = (float *) malloc(sizeof(float) * buffer_width * buffer_height * 3);
    if( image_buffer == NULL )
      fprintf(stderr, "error malloc-ing image_buffer\n");
  }
}
  

void Image::load_from_cimg(CImg<float> *image)
{
  int i = 0;

  allocate_image_buffer(image->width(), image->height());
  
  // Copy the CImg data over into the buffer
  for(int x = 0; x < image->width(); x++) // for each pixel in row x
  {
    for(int y = 0; y < image->height(); y++) 
    {
      image_buffer[i++] = (*image)(x, y, 0, 0); // red component
      image_buffer[i++] = (*image)(x, y, 0, 1); // green component
      image_buffer[i++] = (*image)(x, y, 0, 2); // blue component 
    }
  }
}

void Image::print_pixel(int x, int y)
{
	cout << '(' << image_buffer[3*x] << ',' << image_buffer[3*x+1] << ',' << image_buffer[3*x+2] << ')' << endl;
}

void Image::load_from_file(char *image_path)
{
  CImg<float> *image = new CImg<float>(image_path);
   
  if (image->width() != DEFAULT_WIDTH || image->height() != DEFAULT_HEIGHT) {
     fprintf(stderr, "image width or height are different than expected\n");
     exit(1);
  }
  load_from_cimg(image);

  delete image;

//  i=0;
//  printf("%d components\n", image.spectrum());
//  for(int j=0;j<3*image.width()*image.height();j+=3) {
//    printf("(%d,%d) - %f %f %f\n", i/image.width(), i%image.width(), image_buffer[j], image_buffer[j+1], image_buffer[j+2] );
//    i++;
//  }
  //image.display();

  }

//void Image::randomize_pixels()
//{
//  allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);
//
//	int i = 0;
//	for(int x = 0; x < DEFAULT_WIDTH; x++)
//	{
//		for(int y = 0; y < DEFAULT_HEIGHT; y++)
//		{
//			image_buffer[i++] = (float)rand_one() * 255;
//			image_buffer[i++] = (float)rand_one() * 255;
//			image_buffer[i++] = (float)rand_one() * 255;
//		}
//	}
//}

void Image::randomize_polygons()
{
  for(int poly_index = 0; poly_index < MAX_POLYGONS; poly_index++)
  {
    //int num_points = rand() % (MAX_POINTS - 3) + 3;
    int num_points = MAX_POINTS;

    polygons[poly_index].num_p = num_points;

    polygons[poly_index].color.r = rand_one();
    polygons[poly_index].color.g = rand_one();
    polygons[poly_index].color.b = rand_one();
    polygons[poly_index].color.a = rand_one();

    /*
    int base_x = rand_one() * DEFAULT_WIDTH;
    int base_y = rand_one() * DEFAULT_HEIGHT;
    */

    for(int vertex_index = 0; vertex_index < num_points; vertex_index++)
    {
      polygons[poly_index].points[vertex_index].x = rand_one() * (float)DEFAULT_WIDTH;
      polygons[poly_index].points[vertex_index].y = rand_one() * (float)DEFAULT_HEIGHT;
      polygons[poly_index].points[vertex_index].z = rand_one();


      // Make sure the point doesn't lie outside the bounds of the iamge
      if(polygons[poly_index].points[vertex_index].x < 0)
        polygons[poly_index].points[vertex_index].x = 0;

      if(polygons[poly_index].points[vertex_index].y < 0)
        polygons[poly_index].points[vertex_index].y = 0;

      if(polygons[poly_index].points[vertex_index].x >= DEFAULT_WIDTH)
        polygons[poly_index].points[vertex_index].x = DEFAULT_WIDTH - 1;

      if(polygons[poly_index].points[vertex_index].y >= DEFAULT_HEIGHT)
        polygons[poly_index].points[vertex_index].y = DEFAULT_HEIGHT - 1;
    }
  }
}

void Image::print()
{
  for(int i = 0; i < MAX_POLYGONS; i++)
  {
    cout << "POLYGON" << endl;
    cout << "Color: (" << polygons[i].color.r << "," << polygons[i].color.g << "," << polygons[i].color.b << "," << polygons[i].color.a << ")" << endl;

    cout << "Points" << endl;
    for(int j = 0; j < polygons[i].num_p; j++)
    {
      cout << polygons[i].points[j].x << "," << polygons[i].points[j].y << "," << polygons[i].points[j].z << endl;
    }
  }
}

void Image::render_scanline()
{

  for(int i = 0; i < 3 * DEFAULT_HEIGHT * DEFAULT_WIDTH; i++)
  {
    image_buffer[i] = 0;
  }

  // Loop through all the polygons
  for(int poly_index = 0; poly_index < MAX_POLYGONS; poly_index++)
  {
    // Find the leftmost and rightmost x values
    float min_x = min(polygons[poly_index].points[0].x, min(polygons[poly_index].points[1].x, polygons[poly_index].points[2].x));
    float max_x = max(polygons[poly_index].points[0].x, max(polygons[poly_index].points[1].x, polygons[poly_index].points[2].x));

    float min_y = min(polygons[poly_index].points[0].y, min(polygons[poly_index].points[1].y, polygons[poly_index].points[2].y));
    float max_y = max(polygons[poly_index].points[0].y, max(polygons[poly_index].points[1].y, polygons[poly_index].points[2].y));

    // Loop through all the pixels in the polygon's bounding box
    int buffer_index;
    for(int x = round(min_x); x <= round(max_x); x++)
    {
      for(int y = round(min_y); y <= round(max_y); y++)
      {
        if(pointInTriangle(&polygons[poly_index], x, y))
        {
          buffer_index = 3 * y * DEFAULT_WIDTH + 3 * x;

          // Take into account opacity
          image_buffer[buffer_index]     = (1 - polygons[poly_index].color.a) * 255 * polygons[poly_index].color.r + polygons[poly_index].color.a * image_buffer[buffer_index];
          image_buffer[buffer_index + 1] = (1 - polygons[poly_index].color.a) * 255 * polygons[poly_index].color.g + polygons[poly_index].color.a * image_buffer[buffer_index + 1];
          image_buffer[buffer_index + 2] = (1 - polygons[poly_index].color.a) * 255 * polygons[poly_index].color.b + polygons[poly_index].color.a * image_buffer[buffer_index + 2];
        }
      }
    }
  }
}

//called from display()
//void Image::render_opengl()
//{
//   // printf("render called\n");
//  for(int poly_index = 0; poly_index < MAX_POLYGONS; poly_index++)
//  {
//    glColor4fv( (GLfloat*) &polygons[poly_index].color);
//    //printf("    %f, %f, %f\n", polygons[poly_index].points[poly_index].x, polygons[poly_index].points[poly_index].y, polygons[poly_index].points[poly_index].z);
//
//    glBegin(GL_POLYGON);
//      for(int vertex_index = 0; vertex_index < polygons[poly_index].num_p ; vertex_index++)
//      {
//        glVertex3fv( (GLfloat*) &polygons[poly_index].points[vertex_index]);
//      }
//    glEnd();
//  }
//}

// CImg-based rendering method
//void Image::render_cimg(bool save, char *filename)
//{
//	CImg<float> *render_image = new CImg<float>(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 3);
//
//	render_image->fill(0);
//
//	// Draw each of the polygons
//	// CImg can only draw triangles with built in functions,
//	// so we assume the polygons are triangles
//
//	for(int i = 0; i < MAX_POLYGONS; i++)
//	{
//		float color[3] = {polygons[i].color.r * 255, 
//			polygons[i].color.g * 255,
//			polygons[i].color.b * 255};
//
//		render_image->draw_triangle(
//			polygons[i].points[0].x, polygons[i].points[0].y,
//			polygons[i].points[1].x, polygons[i].points[1].y,
//			polygons[i].points[2].x, polygons[i].points[2].y,
//			color,
//			polygons[i].color.a
//		);
//	}
//
//	if(save == true)
//	{
//		render_image->save(filename);
//	}
//
//	load_from_cimg(render_image);
//    delete render_image;
//}

void Image::save(char *filename)
{

	CImg<float> render_image(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 3);

	int i = 0;
    unsigned char color[3];
	for(int x = 0; x < DEFAULT_WIDTH; x++)
	{
		for(int y = 0; y < DEFAULT_HEIGHT; y++)
		{
			color[0] = (const unsigned char)image_buffer[i++];
            color[1] = (const unsigned char)image_buffer[i++];
            color[2] = (const unsigned char)image_buffer[i++];

			render_image.draw_point(x, y, color);
		}
	}

	render_image.save(filename);
}

void Image::calculate_fitness(Image *target)
{
	fitness = sumOfError(target->image_buffer, DEFAULT_HEIGHT, DEFAULT_WIDTH);
}

void Image::mutate()
{
  int i = (int)(rand_one() * (float)MAX_POLYGONS);

  //if(rand_one() <= MUTATION_PR)
  //{
    int index = (int)(rand_one() * 3.0f);
    
    if(rand_one() < 0.5)
    {
      /*
      if(rand_one() < 0.1)
      {
        for(int v = 0; v < 3; v++)
        {
          polygons[i].points[index].x += rand_range(-20, 20);
          polygons[i].points[index].y += rand_range(-20, 20);
        }
      }
      else
      {
      */
        polygons[i].points[index].x += rand_range(-4, 4);
        polygons[i].points[index].y += rand_range(-4, 4);
      //}
    }
    else
    {
      polygons[i].color.r += rand_range(-0.02, 0.02); 
      polygons[i].color.g += rand_range(-0.02, 0.02);
      polygons[i].color.b += rand_range(-0.02, 0.02);
      polygons[i].color.a += rand_range(-0.02, 0.02);
    }
    
    if(polygons[i].points[index].x < 0) polygons[i].points[index].x = 0;
    if(polygons[i].points[index].y < 0) polygons[i].points[index].y = 0;
    if(polygons[i].color.r < 0) polygons[i].color.r = 0;
    if(polygons[i].color.g < 0) polygons[i].color.g = 0;
    if(polygons[i].color.b < 0) polygons[i].color.b = 0;
    if(polygons[i].color.a < 0) polygons[i].color.a = 0;
    
    if(polygons[i].points[index].x >= DEFAULT_WIDTH) polygons[i].points[index].x = DEFAULT_WIDTH - 1;
    if(polygons[i].points[index].y >= DEFAULT_HEIGHT) polygons[i].points[index].y = DEFAULT_HEIGHT - 1;
    if(polygons[i].color.r > 1) polygons[i].color.a = 1;
    if(polygons[i].color.g > 1) polygons[i].color.g = 1;
    if(polygons[i].color.b > 1) polygons[i].color.b = 1;
    if(polygons[i].color.a > 1) polygons[i].color.a = 1;
  //}
}

//void Image::mutate_pixels()
//{
//	int i = 0;
//	for(int x = 0; x < DEFAULT_WIDTH; x++)
//	{
//		for(int y = 0; y < DEFAULT_HEIGHT; y++)
//		{
//			if(rand_one() < MUTATION_PR)
//			{
//				image_buffer[i * 3] += rand_one() * MUTATION_SPREAD - (MUTATION_SPREAD / 2);
//				image_buffer[i * 3 + 1] += rand_one() * MUTATION_SPREAD - (MUTATION_SPREAD / 2); 
//				image_buffer[i * 3 + 2] += rand_one() * MUTATION_SPREAD - (MUTATION_SPREAD / 2);
//
//				if(image_buffer[i * 3] > 255) image_buffer[i * 3] = 255;
//				if(image_buffer[i * 3 + 1] > 255) image_buffer[i * 3 + 1] = 255;
//				if(image_buffer[i * 3 + 2] > 255) image_buffer[i * 3 + 2] = 255;
//
//				if(image_buffer[i * 3] < 0) image_buffer[i * 3] = 0;
//				if(image_buffer[i * 3 + 1] < 0) image_buffer[i * 3 + 1] = 0;
//				if(image_buffer[i * 3 + 2] < 0) image_buffer[i * 3 + 2] = 0;
//			}
//
//			i++;
//		}
//	}
//}

void Image::set_color(float r, float g, float b)
{
  allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	int i = 0;
	for(int x = 0; x < DEFAULT_WIDTH; x++)
	{
		for(int y = 0; y < DEFAULT_HEIGHT; y++)
		{
			image_buffer[i * 3] = r;
			image_buffer[i * 3 + 1] = g;
			image_buffer[i * 3 + 2] = b;

			i++;
		}
	}
}

//void Image::recombine_pixels(Image* first, Image* second)
//{
//	int i = 0;
//	for(int x = 0; x < DEFAULT_WIDTH; x++)
//	{
//		for(int y = 0; y < DEFAULT_HEIGHT; y++)
//		{
//			if(rand_one() < 0.5)
//			{
//				image_buffer[i * 3] = first->image_buffer[i * 3];
//				image_buffer[i * 3 + 1] = first->image_buffer[i * 3 + 1];
//				image_buffer[i * 3 + 2] = first->image_buffer[i * 3 + 2];
//			}
//			else
//			{
//				image_buffer[i * 3] = second->image_buffer[i * 3];
//				image_buffer[i * 3 + 1] = second->image_buffer[i * 3 + 1];
//				image_buffer[i * 3 + 2] = second->image_buffer[i * 3 + 2];
//			}
//
//			i++;
//		}
//	}
//}

void Image::recombine(Image* first, Image* second)
{
    Image *temp;
	for(int i = 0; i < MAX_POLYGONS; i++)
	{
    
    // Adopt this polygon from either the first or second parent
    if(rand_one() < 0.5)
      temp = first;
    else
      temp = second;

    for(int j = 0; j < 3; j++)
    {
      polygons[i].points[j].x = temp->polygons[i].points[j].x;
      polygons[i].points[j].y = temp->polygons[i].points[j].y;
      polygons[i].points[j].z = temp->polygons[i].points[j].z;
    }

		polygons[i].color.r = temp->polygons[i].color.r;
		polygons[i].color.g = temp->polygons[i].color.g;
		polygons[i].color.b = temp->polygons[i].color.b;
		polygons[i].color.a = temp->polygons[i].color.a;
	}   
}
