#pragma once
#include <vector>
#include "../map_system/map_system.h"
//#include "../ant_system/ant_system.h"
#include "../ant_system_dod/ant_system_dod.h"
using namespace std;


/**
 * @class Controller
 * @brief Sørger for at kontrollere alle logik komponenter korrekt for simulationen
 *
 * 
 * 
 */





enum class SimulationState {
    STOPPED,
    RUNNING,
    PAUSED
};

class Controller
{
private:
    /* data */
    MapSystem map;
    //AntSystem colony;
    AntSystemDOD colony;


    SimulationState state;

    bool nestSet;
    bool ressourceSet;

    float time;

    void updateFrameInternal(); 

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

    void drawObstacles(const std::vector<pair<int,int>>& points);


    const MapSystem& getMap() const {return map;}
    //const AntSystem& getColony() const {return colony;}

    const AntSystemDOD& getColony() const {return colony;}

    SimulationState getState() const {return state;}

};


