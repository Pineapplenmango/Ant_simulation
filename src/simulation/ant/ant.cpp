#include "ant.h"
#include <cmath>
#include <algorithm>


Ant::Ant(int startX, int StartY) : positionX(startX), positionY(StartY), carryingRessource(false), alpha(2.0f), beta(0.5f)
{
    path.clear();
}

Ant::~Ant()
{
}

void Ant::update(MapSystem& map)
{

    if(path.size()>1000){
                carryingRessource = false;
                positionX = map.getNestX();
                 positionY = map.getNestY();
                path.clear();
              
            
        }


     //Hvis den leder efter ressource
    if (!carryingRessource)
    {

        if (!carryingRessource &&
                positionX == map.getResourceX() &&
                 positionY == map.getResourceY())
                {
                    carryingRessource = true;
                        return;
                        }

        move(map);

    }
    // hvis den har fundet ressource
    else
    {
                float distToNest = std::hypot(positionX - map.getNestX(), positionY - map.getNestY());
                float k = 0.08f;             // hvor hurtigt feromonet vokser med afstand
                float base = 0.02f;          // maksimal afsætning i den fjerne ende
                float strength = base * (1.0f - std::exp(-k * distToNest));
                map.addPheromone(positionX, positionY, strength);

        

        if (!path.empty()) {
            auto [px, py] = path.back(); path.pop_back();
         positionX = px; positionY = py;
        } else {
    if (std::abs(positionX - map.getNestX()) <= 1 &&
        std::abs(positionY - map.getNestY()) <= 1) {
         map.ressourcesDelivered++;
        carryingRessource = false;
            } else {
        // Mistet vej hjem, drop feromon og vend tilbage til søgning
        carryingRessource = false;
        path.clear();
            }
        }




    }

}


std::pair<int,int> Ant::chooseNextCell(MapSystem& map)
{
    auto neighbors = map.findNeighbors(positionX, positionY);
    if (neighbors.empty()) return {positionX, positionY};

    // 1) Korttidshukommelse
    const int memoryLength = 6;
    int startIdx = std::max(0, (int)path.size() - memoryLength);
    for (int i = startIdx; i < (int)path.size(); ++i)
    {
        auto [hx, hy] = path[i];
        neighbors.erase(
            std::remove_if(neighbors.begin(), neighbors.end(),
                           [&](const std::pair<int,int>& n)
                           { return n.first == hx && n.second == hy; }),
            neighbors.end());
    }
    if (neighbors.empty())
    {
        neighbors = map.findNeighbors(positionX, positionY);
    }
    //2) Forrige retning (unit-vektor)
    float dirx = 0.0f, diry = 0.0f;
    if (!path.empty()) {
        dirx = (float)(positionX - path.back().first); //path er pair(x,y), .first er derved x
        diry = (float)(positionY - path.back().second);
        float len = std::sqrt(dirx*dirx + diry*diry);
        if (len > 0.0f) { dirx /= len; diry /= len; }
    }

    const float directionBiasStrength = 0.7f; // du kan tune 0.4–1.5

    // 3) Dorigo-vægtning + retningsbias
    std::vector<float> weights;
    weights.reserve(neighbors.size());
    float total = 0.0f;

    for (auto [nx, ny] : neighbors)
    {
       float tau = map.getCell(nx, ny).getPheromone();
    if (tau < 1e-6f) tau = 1e-6f; // undgå nulvægt

    // retningsbias via cos mellem forrige- og næste mulige position
    float stepx = (float)(nx - positionX);
    float stepy = (float)(ny - positionY);
    float slen = std::sqrt(stepx*stepx + stepy*stepy);
    if (slen > 0.0f) { stepx /= slen; stepy /= slen; }
    float cosTheta = stepx * dirx + stepy * diry; // [-1..1]
    float bias = 1.0f + directionBiasStrength * cosTheta;
    if (bias < 0.1f) bias = 0.1f; // undgå nulretning

    // Beta styrer hvor stærkt feromonet dominerer:
    // høj beta → følger trail blindt
    // lav beta → mere tilfældig og udforskende
    float tauInfluence = std::pow(tau, alpha / beta);

    //Tilføj tilfældig støj 
    float noise = 0.8f + ((float)rand() / RAND_MAX) * (0.4f / beta); 
    // Lav beta = mere støj = mere udforskning

    float value = tauInfluence * bias * noise;
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




