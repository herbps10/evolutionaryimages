class Image
{
  //I moved public: to the top for debugging purposes
  public:
  float *image_buffer;
  float *polygon_buffer;
  polygon polygons[MAX_POLYGONS];
  // private error calculating functions to be called by fitness
  double sumOfError(float *buffer, int height, int width); // compares buffer to image_buffer and sums the error
  double sumOfSquaresError(float *buffer, int height, int width); // compares buffer to image_buffer and sums the square of the error
  double fitness;

  //public:
    Image();
    void load_from_cimg(CImg<float>);
    void load_from_file(char *);
    void randomize_polygons();
    void allocate_polygons();
    void print();
    void render();
    void render_cimg(bool, char *);
    void calculate_fitness(Image);
    void mutate();
    Image* recombine(Image);
};
