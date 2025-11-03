#include "cell.h"

Cell::Cell(/* args */): walkable(true), pheromone(0.0f) // alle celler starter med at være walkable og intet pheromone
{


}

Cell::~Cell()
{
}



void Cell::setPheromone(float value) {

    pheromone = (value < 0.0f) ? 0.0f : value; //feromon kan ikke være negativt
}



