// assumes image_buffer and buffer are the same size
double Image::sumOfError(float *buffer, int height, int width){
    int i;
    double error = 0.0;

    if( height != DEFAULT_HEIGHT || width != DEFAULT_WIDTH ) {
        fprintf(stderr, "sumOfError: buffers are not the same size.\n");
        return -1.0;
    }

    for(i=0;i<DEFAULT_HEIGHT*DEFAULT_WIDTH*3;i++) {
        error += image_buffer[i] < buffer[i] ? buffer[i] - image_buffer[i] : image_buffer[i] - buffer[i];
    }
    
    return error;
}


// assumes image_buffer and buffer are the same size
double Image::sumOfSquaresError(float *buffer, int height, int width){
    int i;
    double error = 0.0, diff;

    if( height != DEFAULT_HEIGHT || width != DEFAULT_WIDTH ) {
        fprintf(stderr, "sumOfSquaresError: buffers are not the same size.\n");
        return -1.0;
    }

    for(i = 0; i < DEFAULT_HEIGHT * DEFAULT_WIDTH * 3; i++) {
        diff = image_buffer[i] - buffer[i];

        error += diff*diff;
    }

    // Normalize to [0, 1]
    error = error / (3 * 255 * DEFAULT_HEIGHT * DEFAULT_WIDTH);
    
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

void Image::allocate_image_buffer(int width, int height)
{
  if(image_buffer == NULL)
  {
    // Allocate space to the target image buffer
    image_buffer = (float *) malloc(sizeof(float) * width * height * 3);
    if( image_buffer == NULL )
      fprintf(stderr, "error malloc-ing image_buffer\n");
  }
}
  

void Image::load_from_cimg(CImg<float> image)
{
  int i = 0;

  allocate_image_buffer(image.width(), image.height());
  
  // Copy the CImg data over into the buffer
  for(int y = image.height() - 1; y >= 0; y--) // for each row bottom to top (to allign with OpenGl getPixelValues(...))
  {
    for(int x = 0; x < image.width(); x++) // for each pixel in row x
    {
      image_buffer[i++] = image(x, y, 0, 0); // red component
      image_buffer[i++] = image(x, y, 0, 1); // green component
      image_buffer[i++] = image(x, y, 0, 2); // blue component 
    }
  }
}

void Image::print_pixel(int x, int y)
{
	cout << '(' << image_buffer[3*x] << ',' << image_buffer[3*x+1] << ',' << image_buffer[3*x+2] << ')' << endl;
}

void Image::load_from_file(char *image_path)
{
  CImg<float> image(image_path);
   
  load_from_cimg(image);

//  i=0;
//  printf("%d components\n", image.spectrum());
//  for(int j=0;j<3*image.width()*image.height();j+=3) {
//    printf("(%d,%d) - %f %f %f\n", i/image.width(), i%image.width(), image_buffer[j], image_buffer[j+1], image_buffer[j+2] );
//    i++;
//  }
  //image.display();

  }

void Image::randomize_pixels()
{
  allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	int i = 0;
	for(int x = 0; x < DEFAULT_WIDTH; x++)
	{
		for(int y = 0; y < DEFAULT_HEIGHT; y++)
		{
			image_buffer[i++] = (float)rand_one() * 255;
			image_buffer[i++] = (float)rand_one() * 255;
			image_buffer[i++] = (float)rand_one() * 255;
		}
	}
}

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

    for(int vertex_index = 0; vertex_index < num_points; vertex_index++)
    {
      polygons[poly_index].points[vertex_index].x = rand_one();
      polygons[poly_index].points[vertex_index].y = rand_one();
      polygons[poly_index].points[vertex_index].z = rand_one();
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

//called from display()
void Image::render()
{
   // printf("render called\n");
  for(int poly_index = 0; poly_index < MAX_POLYGONS; poly_index++)
  {
    glColor4fv( (GLfloat*) &polygons[poly_index].color);
    //printf("    %f, %f, %f\n", polygons[poly_index].points[poly_index].x, polygons[poly_index].points[poly_index].y, polygons[poly_index].points[poly_index].z);

    glBegin(GL_POLYGON);
      for(int vertex_index = 0; vertex_index < polygons[poly_index].num_p ; vertex_index++)
      {
        glVertex3fv( (GLfloat*) &polygons[poly_index].points[vertex_index]);
      }
    glEnd();
  }
}

// CImg-based rendering method
void Image::render_cimg(bool save, char *filename)
{
	CImg<float> render_image(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 3);

	render_image.fill(0);

	// Draw each of the polygons
	// CImg can only draw triangles with built in functions,
	// so we assume the polygons are triangles

	for(int i = 0; i < MAX_POLYGONS; i++)
	{
		float color[3] = {polygons[i].color.r * 255, 
			polygons[i].color.g * 255,
			polygons[i].color.b * 255};

		render_image.draw_triangle(
			polygons[i].points[0].x * DEFAULT_WIDTH, polygons[i].points[0].y * DEFAULT_HEIGHT,
			polygons[i].points[1].x * DEFAULT_WIDTH, polygons[i].points[1].y * DEFAULT_HEIGHT,
			polygons[i].points[2].x * DEFAULT_WIDTH, polygons[i].points[2].y * DEFAULT_HEIGHT,
			color,
			polygons[i].color.a
		);
	}

	if(save == true)
	{
		render_image.save(filename);
	}

	load_from_cimg(render_image);
}

void Image::save(char *filename)
{

	CImg<float> render_image(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 3);

	int i = 0;
	for(int x = 0; x < DEFAULT_WIDTH; x++)
	{
		for(int y = 0; y < DEFAULT_HEIGHT; y++)
		{
			const unsigned char color[] = { (const unsigned char)image_buffer[i * 3], (const unsigned char)image_buffer[i * 3 + 1], (const unsigned char)image_buffer[i * 3 + 2] };

			render_image.draw_point(x, y, color);

			i++;
		}
	}

	render_image.save(filename);
}

void Image::calculate_fitness(Image * target)
{
	fitness = sumOfSquaresError(target->image_buffer, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

void Image::mutate()
{
	for(int i = 0; i < MAX_POLYGONS; i++)
	{
		if(rand_one() < MUTATION_PR)
		{
			int index = (random() / RAND_MAX) * 3;
			
			polygons[i].points[index].x += rand_one() - 0.5;
			polygons[i].points[index].y += rand_one() - 0.5;
			
			polygons[i].color.r += rand_one() - 0.5;
			polygons[i].color.g += rand_one() - 0.5;
			polygons[i].color.b += rand_one() - 0.5;
			polygons[i].color.a += rand_one() - 0.5;
			
			if(polygons[i].points[index].x < 0) polygons[i].points[index].x = 0;
			if(polygons[i].points[index].y < 0) polygons[i].points[index].y = 0;
			if(polygons[i].color.r < 0) polygons[i].color.a = 0;
			if(polygons[i].color.g < 0) polygons[i].color.g = 0;
			if(polygons[i].color.b < 0) polygons[i].color.b = 0;
			
			if(polygons[i].points[index].x > 1) polygons[i].points[index].x = 1;
			if(polygons[i].points[index].y > 1) polygons[i].points[index].y = 1;
			if(polygons[i].color.r > 1) polygons[i].color.a = 1;
			if(polygons[i].color.g > 1) polygons[i].color.g = 1;
			if(polygons[i].color.b > 1) polygons[i].color.b = 1;
		}
	}
}

void Image::mutate_pixels()
{
	int i = 0;
	for(int x = 0; x < DEFAULT_WIDTH; x++)
	{
		for(int y = 0; y < DEFAULT_HEIGHT; y++)
		{
			if(rand_one() < MUTATION_PR)
			{
				image_buffer[i * 3] += rand_one() * MUTATION_SPREAD - (MUTATION_SPREAD / 2);
				image_buffer[i * 3 + 1] += rand_one() * MUTATION_SPREAD - (MUTATION_SPREAD / 2); 
				image_buffer[i * 3 + 2] += rand_one() * MUTATION_SPREAD - (MUTATION_SPREAD / 2);

				if(image_buffer[i * 3] > 255) image_buffer[i * 3] = 255;
				if(image_buffer[i * 3 + 1] > 255) image_buffer[i * 3 + 1] = 255;
				if(image_buffer[i * 3 + 2] > 255) image_buffer[i * 3 + 2] = 255;

				if(image_buffer[i * 3] < 0) image_buffer[i * 3] = 0;
				if(image_buffer[i * 3 + 1] < 0) image_buffer[i * 3 + 1] = 0;
				if(image_buffer[i * 3 + 2] < 0) image_buffer[i * 3 + 2] = 0;
			}

			i++;
		}
	}
}

void Image::set_color(float r, float g, float b)
{
  allocate_image_buffer(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	int i = 0;
	for(int x = 0; x < DEFAULT_WIDTH; x++)
	{
		for(int y = 0; y < DEFAULT_HEIGHT; y++)
		{
			image_buffer[i * 3] = 255;
			image_buffer[i * 3 + 1] = 255;
			image_buffer[i * 3 + 2] = 255;

			i++;
		}
	}
}

void Image::recombine_pixels(Image* first, Image* second)
{
	int i = 0;
	for(int x = 0; x < DEFAULT_WIDTH; x++)
	{
		for(int y = 0; y < DEFAULT_HEIGHT; y++)
		{
			if(rand_one() < 0.5)
			{
				image_buffer[i * 3] = first->image_buffer[i * 3];
				image_buffer[i * 3 + 1] = first->image_buffer[i * 3 + 1];
				image_buffer[i * 3 + 2] = first->image_buffer[i * 3 + 2];
			}
			else
			{
				image_buffer[i * 3] = second->image_buffer[i * 3];
				image_buffer[i * 3 + 1] = second->image_buffer[i * 3 + 1];
				image_buffer[i * 3 + 2] = second->image_buffer[i * 3 + 2];
			}

			i++;
		}
	}
}

Image* Image::recombine(Image* second)
{
	Image *result = new Image();
	
	for(int i = 0; i < MAX_POLYGONS; i++)
	{
		for(int j = 0; j < MAX_POINTS; j++)
		{
			result->polygons[i].points[j].x = polygons[i].points[j].x;
			result->polygons[i].points[j].y = polygons[i].points[j].y;
		}
		
		result->polygons[i].color.r = second->polygons[i].color.r;
		result->polygons[i].color.g = second->polygons[i].color.g;
		result->polygons[i].color.b = second->polygons[i].color.b;
		result->polygons[i].color.a = second->polygons[i].color.a;
	}   
	
	return result;
   
}

bool pointInTriangle(polygon *triangle, float x, float y)
{
  float y1 = triangle->points[0].y;
  float y2 = triangle->points[1].y;
  float y3 = triangle->points[2].y;

  float x1 = triangle->points[0].x;
  float x2 = triangle->points[1].x;
  float x3 = triangle->points[2].x;

  float lambda[3];

  //Convert to barycentric coordinate system
  lambda[0] = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / ((y2 - y3)*(x1 - x3) + (x3 - x2) * (y1 - y3));

  lambda[1] = ((y3 - y1)*(x - x3) + (x1 - x3)*(y-y3)) / ((y2 - y3)*(x1-x3) + (x3 - x2)*(y1 - y3));

  lambda[2] = 1 - lambda[0] - lambda[1];

  // If lambda 1, 2, and 3 are between 0 and 1, then the point x,y is in the triangle

  for(int i = 0; i < 3; i++)
  {
    if(lambda[i] < 0 || lambda[i] > 1)
    {
      return false;
    }
  }

  return true;
};
