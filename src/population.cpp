Population::Population(Image *target)
{
  this->target = target;

  new_individual = new Image();
  new_individual->allocate_image_buffer(target->width(), target->height());
  
  elitism = 30;

  iteration = 0;

  // Generate the population
  for(int i = 0; i < POPULATION_SIZE; i++)
  {
    population[i] = new Image();
    population[i]->allocate_image_buffer(target->width(), target->height());

    population[i]->randomize_polygons();
    population[i]->render_scanline();
    population[i]->calculate_fitness(target); 
  }

  // Sort the population
  sort(population, population + POPULATION_SIZE, compare);
}

Population::~Population()
{
  for(int i = 0; i < POPULATION_SIZE; i++) {
      delete(population[i]);
  }
}

void Population::iterate()
{
  int i1 = rand_one() * elitism;
  int i2 = rand_one() * elitism;


  this->new_individual->recombine(population[i1], population[i2]);
  this->new_individual->mutate();

  if( rand_one() < 0.4f ) {
    this->new_individual->randomize_a_back_polygon_and_move_to_front();
  	//cout << this << " " << this->new_individual->image_buffer << " " << this->new_individual->image_buffer[0] << "\n" << endl;
  }

  this->new_individual->render_scanline();
  this->new_individual->calculate_fitness(this->target);

  this->test_new_individual();
}

void Population::save_individual(int n, char *filename)
{
  this->population[n]->save(filename);
}

Image *Population::get(int n)
{
  return this->population[n];
}

void Population::add_from_buffer(float *buffer)
{
  new_individual->unpack(buffer);
  this->new_individual->render_scanline();
  this->new_individual->calculate_fitness(this->target);

  // If the new individual is better than our worst, it will get added to the population
  this->test_new_individual();
}

void Population::test_new_individual()
{
  // If the new individual is better than the worst in the current population,
  // replace the worst one with the new one
  if(new_individual->fitness < population[POPULATION_SIZE - 1]->fitness)
  {
    // Replace the worst individual with the new one
    temp = new_individual;
    new_individual = population[POPULATION_SIZE - 1];
    population[POPULATION_SIZE - 1] = temp;

    // Bubble the new individual up until it is in the right spot
    int index = POPULATION_SIZE - 1;
    while(index > 0 && population[index - 1]->fitness > population[index]->fitness)
    {
      temp = population[index - 1];
      population[index - 1] = population[index];
      population[index] = temp;

      index--;
    }
  }
}
