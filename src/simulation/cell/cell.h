#pragma once

/**
 * @class Cell
 * @brief Mindste enhed i simulationens grid.
 *
 * En celle kan enten være:
 *  - walkable (myrer kan passere)
 *  - blokeret (forhindring)
 *
 * Derudover indeholder cellen et feromonniveau, som bruges
 * af myrer til at træffe bevægelsesbeslutninger.
 *
 */


class Cell
{
private:

    bool walkable; ///< True hvis myrer kan bevæge sig gennem cellen.
    float pheromone;  ///< Feromonniveauet i cellen.
public:
    Cell();
    ~Cell();

    /// Walkable-flag
    inline bool isWalkable() const {return walkable;} 
    inline void setWalkable(bool value) {walkable = value;}

     /// Feromon-access
    inline float getPheromone() const {return pheromone;}
    void setPheromone(float value);



};

