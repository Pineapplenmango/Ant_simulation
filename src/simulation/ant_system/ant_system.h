#pragma once
#include <vector>
#include "../ant/ant.h"
using namespace std;


/**
 * @class AntSystem
 * @brief Sørger for alt myre relateret logik kan tilgåes af controller
 *
 */



class AntSystem
{
private:
    /* data */
    std::vector<Ant> ants;

public:
    AntSystem(/* args */);
    ~AntSystem();

    void spawnAnt(int x, int y);
    void removeAnt();
    void updateAll(MapSystem& map, float time);

    const std::vector<Ant>& getAnts() const { return ants; } 
};


