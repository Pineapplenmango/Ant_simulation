#pragma once
#include <vector>
#include "../map_system/map_system.h"



/**
 * @class AntSystemDOD
 * @brief Data-Oriented version af myresystemet.
 *
 * I stedet for individuelle objekter lagres al myredata i
 * parallelle arrays (Structure of Arrays).
 *
 * Dette forbedrer:
 *  - Cache locality
 *  - SIMD- og CPU-forudsigelighed
 *  - Skalerbarhed ved mange myrer
 */

class AntSystemDOD
{
private:
  
    //hot data (bruges hver frame)
    std::vector<int> positionX;
    std::vector<int> positionY;
    std::vector<float> alpha;
    std::vector<float> beta;

    /// Bevægehistorik (per myre)
    std::vector<std::vector<std::pair<int,int>>> path;

    //cold data
    std::vector<bool> carryingRessource;


    /**
     * @brief Opdaterer én myre ud fra dens index.
     */
    void updateAnt(int i, MapSystem& map);    

    /**
     * @brief Vælger næste celle for myre i.
     */
    std::pair<int,int> chooseNextCell(int i,MapSystem& map);


    /**
     * @brief Flytter myre i og opdaterer path.
     */
    void move(int i,MapSystem& map);

    

public:
    AntSystemDOD(/* args */);
    ~AntSystemDOD();

    void spawnAnt(int x, int y);
    void removeAnt();


    /**
     * @brief Opdaterer alle myrer sekventielt
     */
    void update(MapSystem& map);

     /// Getters til rendering
    int getCount() const { return (int)positionX.size(); }
    int getX(int i) const { return positionX[i]; }
    int getY(int i) const { return positionY[i]; }

};


