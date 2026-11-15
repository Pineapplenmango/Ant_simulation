#pragma once
#include <vector>
#include "../ant/ant.h"



/**
 * @class AntSystem
 * @brief Samler og styrer alle myre-objekter (OOP-design).
 *
 * AntSystem fungerer som et simpelt container-objekt for Ant-instanser.
 * Det er ansvarligt for at oprette, fjerne og opdatere alle myrer pr. frame.
 *
 * Dette design benytter objekt-orienteret tilgang, hvor hver myre
 * indeholder sin egen data og logik.
 */

class AntSystem
{
private:
    /// Liste over alle myrer i simulationen
    std::vector<Ant> ants;

public:
    AntSystem(/* args */);
    ~AntSystem();

 /**
     * @brief Opretter en ny myre ved en given position.
     * @param x Startposition X (typisk reden)
     * @param y Startposition Y
     */
    void spawnAnt(int x, int y);

  /**
     * @brief Fjerner den senest tilføjede myre.
     */
    void removeAnt();

/**
     * @brief Opdaterer alle myrer i systemet.
     * @param map Reference til MapSystem, som myrerne bevæger sig i
     */

    void updateAll(MapSystem& map);


     /**
     * @brief Giver læseadgang til myrelisten (fx rendering).
     */
    const std::vector<Ant>& getAnts() const { return ants; } 
};


