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
    ~Image();
    void print_pixel(int, int);
    void load_from_cimg(CImg<float>);
    void load_from_file(char *);
    void randomize_polygons();
    void randomize_pixels();
    void allocate_polygons();
    void allocate_image_buffer(int, int);
    void print();
    void render();
    void render_cimg(bool, char *);
    void calculate_fitness(Image*);
    void mutate();
    Image* recombine(Image*);
    void recombine_pixels(Image*, Image*);
    void mutate_pixels();
    void set_color(float r, float g, float b);
    void save(char *);
};
