#pragma once

/**
 * @class Cell
 * @brief Repræsenterer en enkelt celle i griddet.
 *
 * En celle kan være bevægelig eller blokeret, og kan indeholde et feromon-niveau.
 * Denne klasse anvender `inline` og `const` for at minimere kald-overhead.
 */


class Cell
{
private:

    bool walkable; ///< True hvis myrer kan bevæge sig gennem cellen.
    float pheromone;  ///< Feromonniveauet i cellen.
public:
    Cell(/* args */);
    ~Cell();

    inline bool isWalkable() const {return walkable;} 
    inline void setWalkable(bool value) {walkable = value;}

    inline float getPheromone() const {return pheromone;}
    void setPheromone(float value);



};

