#ifndef POPULATION_HEADER
#define POPULATION_HEADER

class Population
{
  public:
	Population(Image *target);
	~Population();

  void iterate();
  void save_individual(int n, char *filename);
  Image *get(int n);

  void add_from_buffer(float *buffer);
  void test_new_individual();

	private:
	Image *population[POPULATION_SIZE];

  Image *target;
	Image *new_individual;
  Image *temp;

  int elitism;
  int iteration; // Number of iterations performed

  void bubble();

};

#endif
