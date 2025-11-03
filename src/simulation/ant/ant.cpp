#include "ant.h"
#include <cmath>


Ant::Ant(int startX, int StartY) : positionX(startX), positionY(StartY), carryingRessource(false), alpha(1.0f), beta(1.0f)
{
    path.clear();
}

Ant::~Ant()
{
}

void Ant::update(MapSystem& map)
{

     //Hvis den leder efter ressource
    if (!carryingRessource)
    {
        // tjek om position er ressource
        if (positionX == map.getResourceX() && positionY == map.getResourceY()){
            carryingRessource = true;
            return; // begynder at backtracke i næste frame
        }

        move(map);

        // tjek igen om position er ressource
        if (positionX == map.getResourceX() && positionY == map.getResourceY()){
            carryingRessource = true;
            return;
        }
    }
    // hvis den har fundet ressource
    else
    {
        map.addPheromone(positionX, positionY, 0.05f);

        if (!path.empty()){
            auto [px,py] = path.back(); path.pop_back();
            positionX = px; positionY = py;
        } else {
            // tjek om position er nest
            if (positionX == map.getNestX() && positionY == map.getNestY()){
                carryingRessource = false; // ny tur
            }
        }
    }

}


std::pair<int,int> Ant::chooseNextCell(MapSystem& map)
{
    auto neighbors = map.getNeighbors(positionX, positionY);
    if (neighbors.empty()) return {positionX, positionY};

    std::vector<float> weights;
    float total = 0.0f;

    for (auto [nx, ny] : neighbors)
    {
        float tau = map.getCell(nx, ny).getPheromone();
        float eta = 1.0f;
        float value = pow(tau, alpha) * pow(eta, beta);
        weights.push_back(value);
        total += value;
    }

    if (total <= 0.0f)
        return neighbors[rand() % neighbors.size()];

    float r = static_cast<float>(rand()) / RAND_MAX;
    float cumulative = 0.0f;
    for (size_t i = 0; i < neighbors.size(); ++i)
    {
        cumulative += weights[i] / total;
        if (r <= cumulative)
            return neighbors[i];
    }

    return neighbors.back();
}



void Ant::move(MapSystem& map)
{
    auto [nextX, nextY] = chooseNextCell(map);
    path.push_back({positionX, positionY});
    positionX = nextX;
    positionY = nextY;
}




