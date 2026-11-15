#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include "../cell/cell.h"
#include "../map_system/map_system.h"



/**
 * @class Ant
 * @brief Repræsenterer én individuel myre i simulationen.
 *
 * Myren bevæger sig på et grid og vælger sin retning baseret på:
 *  - Feromonstyrke
 *  - Retningsbias (tidligere bevægelse)
 *  - Tilfældig støj (udforskning)
 *
 * Når myren finder en ressource, følger den sin gemte sti tilbage til reden
 * og afsætter feromoner proportionalt med afstanden.
 */




class Ant
{
private:
     /// Aktuel position
    int positionX;
    int positionY;

    /// Om myren bærer en ressource
    bool carryingRessource;

    /// Historik over tidligere positioner (bruges som hukommelse)
    std::vector<std::pair<int,int>> path;

    /// ACO-parametre
    float alpha;///< Feromonindflydelse
    float beta; ///< Støjniveau / udforskning

    /**
     * @brief Vælger næste celle baseret på Dorigo-vægtning.
     */
    std::pair<int,int> chooseNextCell(MapSystem& map);
  

    /**
     * @brief Flytter myren én celle og gemmer den tidligere position.
     */
    void move(MapSystem& map);


public:
    Ant(int startX, int StartY);

    

    ~Ant();

    /**
     * @brief Opdaterer myrens tilstand for én simulation-frame.
     */

    void update(MapSystem& map);

    /// Getter til rendering
    inline int getX() const { return positionX; }
    inline int getY() const { return positionY; }


};


