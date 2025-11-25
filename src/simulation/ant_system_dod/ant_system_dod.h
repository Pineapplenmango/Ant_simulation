#pragma once
#include <vector>
#include "../map_system/map_system.h"
using namespace std;


/**
 * @class AntSystemDOD
 * @brief Sørger for alt myre relateret logik kan tilgåes af controller
 *
 */



class AntSystemDOD
{
private:
    /* data */
    //hot data
    std::vector<int> positionX;
    std::vector<int> positionY;
    //std::vector<int> pathX;
    //std::vector<int> pathY;
    std::vector<std::vector<std::pair<int,int>>> path;

    //cold data
    std::vector<bool> carryingRessource;

    //very cold data
    std::vector<float> alpha;
    std::vector<float> beta;

    //hjælper funktioner
    void updateAnt(int i, MapSystem& map);    
    std::pair<int,int> chooseNextCell(int i,MapSystem& map);
    void move(int i,MapSystem& map);

    

public:
    AntSystemDOD(/* args */);
    ~AntSystemDOD();

    void spawnAnt(int x, int y);
    void removeAnt();

    void update(MapSystem& map);

    int getCount() const { return (int)positionX.size(); }
    int getX(int i) const { return positionX[i]; }
    int getY(int i) const { return positionY[i]; }

};


