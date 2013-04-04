#ifndef IMAGE_HEADER
#define IMAGE_HEADER

class Image
{
  //private:
  public:

  float *image_buffer;
  //float *polygon_buffer;


  // private error calculating functions to be called by fitness
  double sumOfError(float *buffer, int height, int width); // compares buffer to image_buffer and sums the error
  double sumOfSquaresError(float *buffer, int height, int width); // compares buffer to image_buffer and sums the square of the error

  int image_width;
  int image_height;

  public: 

  polygon polygons[MAX_POLYGONS];
  double fitness;

  // Construct, destructor
  Image();
  ~Image();

  // Property accessors
  int width();
  int height();
  
  // Loader functions
  void load_from_cimg(CImg<float>*);
  void load_from_file(char *);

  // Randomization
  void randomize_polygons();
  void randomize_pixels();

  // Memory allocation
  void allocate_polygons();
  void allocate_image_buffer(int, int);

  // Render functions
  void render_scanline();
  void render_opengl();
  void render_cimg(bool, char *);

  void calculate_fitness(Image*);

  // Genetic operators
  void recombine(Image*, Image*);
  void recombine_pixels(Image*, Image*);
  void mutate();
  void mutate_pixels();

  void randomize_a_back_polygon_and_move_to_front();

  void set_color(float r, float g, float b);

  // IO
  void print();
  void print_pixel(int, int);
  void save(char *);

  int pack_size();
  float *pack();
  void unpack(float *);
};

#endif
