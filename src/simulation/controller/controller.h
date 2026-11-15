#pragma once
#include <vector>
#include "../map_system/map_system.h"
//#include "../ant_system/ant_system.h"
#include "../ant_system_dod/ant_system_dod.h"




/**
 * @enum SimulationState
 * @brief Definerer simulationens overordnede tilstand.
 */

enum class SimulationState {
    STOPPED,
    RUNNING,
    PAUSED
};



/**
 * @class Controller
 * @brief Overordnet styring af simulationen.
 *
 * Controller fungerer som facade mellem UI og systems:
 *  - MapSystem (grid, feromoner, ressourcer)
 *  - AntSystem / AntSystemDOD
 *
 * Den håndterer også simulationens livscyklus (start/pause/stop).
 */

class Controller
{
private:
    /* data */
    MapSystem map; ///< Simulationens grid
    //AntSystem colony; 
    AntSystemDOD colony; ///< Myrekoloni (DOD-version)


    SimulationState state;

    bool nestSet;
    bool ressourceSet;

    float time; ///< Simuleret tid

    /// Intern frame-opdatering
    void updateFrameInternal();  

    /// Tjekker om simulationen er klar til start
    bool readyToStart() const {return nestSet && ressourceSet;}

public:

    Controller(/* args */);
    ~Controller();
    
    void placeNestPoint(int x, int y);
    void placeRessourcePoint(int x, int y);


    void start();
    void stop();
    void pause();
    void updateFrame();

    
    void addAnts(int amount);
    void removeAnts(int amount);

    void drawObstacles(const std::vector<std::pair<int,int>>& points);


    const MapSystem& getMap() const {return map;}
    //const AntSystem& getColony() const {return colony;}

    const AntSystemDOD& getColony() const {return colony;}

    SimulationState getState() const {return state;}

};


