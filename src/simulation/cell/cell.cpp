#include "cell.h"

Cell::Cell(/* args */): walkable(true), pheromone(0.0f) // alle celler starter med at være walkable og intet pheromone
{


}

Cell::~Cell()
{
}



void Cell::setPheromone(float value) {

   // pheromone = (value < 0.0f) ? 0.0f : value; //feromon kan ikke være negativt


    const float MAX_PHEROMONE = 10.0f; // du kan justere denne værdi
    if (value < 0.0f) value = 0.0f;
    if (value > MAX_PHEROMONE) value = MAX_PHEROMONE;
    pheromone = value;



}



