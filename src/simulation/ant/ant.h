#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include "../cell/cell.h"
#include "../map_system/map_system.h"
using namespace std;


/**
 * @class Ant
 * @brief Repræsenterer en individuel myre i simulationen.
 *
 * Anten bevæger sig på et grid, påvirket af feromonstyrker og tilfældige valg.
 * Hvis den finder en ressource, vil den bevæge sig tilbage mod reden og aflejre feromoner på sin vej.
 *
 * @param positionX Myrens nuværende X-position i gridet.
 * @param positionY Myrens nuværende Y-position i gridet.
 * @param carryingRessource True, hvis myren bærer en ressource tilbage.
 */





class Ant
{
private:
    int positionX;
    int positionY;
    bool carryingRessource;
    std::vector<pair<int,int>> path; // historik af rute
    //std::vector<pair<int,int>> setPath;
    float alpha;
    float beta;

    std::pair<int,int> chooseNextCell(MapSystem& map);
  



    void move(MapSystem& map);


public:
    Ant(/* args */);
    Ant(int startX, int StartY);

    

    ~Ant();
    void update(MapSystem& map);

    inline int getX() const { return positionX; }
    inline int getY() const { return positionY; }


};


