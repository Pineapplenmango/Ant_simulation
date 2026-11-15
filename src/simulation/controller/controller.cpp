#include "controller.h"
#include <iostream>

Controller::Controller(/* args */) : state(SimulationState::STOPPED),
      nestSet(false),
      ressourceSet(false),
      time(0.0f)
{}

Controller::~Controller()
{
}

void Controller::placeNestPoint(int x, int y)
{
    if(!map.setNestPoint(x, y))
    {
        std::cout << "Failed to place nest at (" << x << ", " << y << ")\n";
        return;
    }
    nestSet = true;
    std::cout << "Nest placed at (" << x << ", " << y << ")\n";
}

void Controller::placeRessourcePoint(int x, int y)
{
    if(!map.setRessourcePoint(x, y))
    {
        std::cout << "Failed to place resource at (" << x << ", " << y << ")\n";
        return;
    }
    ressourceSet = true;
    std::cout << "Resource placed at (" << x << ", " << y << ")\n";
}

void Controller::addAnts(int amount)
{
    if (!nestSet) {
        std::cout << "Cannot add ants: Nest not set.\n";
        return;
    }

    for (int i = 0; i < amount; ++i)
        colony.spawnAnt(map.getNestX(), map.getNestY());

    std::cout << amount << " ants added.\n";
}

void Controller::removeAnts(int amount)
{
    for (int i = 0; i < amount; ++i)
        colony.removeAnt();

    std::cout << amount << " ants removed.\n";
}

void Controller::drawObstacles(const std::vector<std::pair<int,int>>& points)
{
    for (auto& p : points)
        map.setObstacle(p.first, p.second);

    std::cout << points.size() << " obstacles drawn.\n";
}

void Controller::start()
{
    if (!readyToStart()) {
        std::cout << "Cannot start simulation: Nest or resource not set.\n";
        return;
    }

    if (state == SimulationState::STOPPED || state == SimulationState::PAUSED) {
        state = SimulationState::RUNNING;
        std::cout << "Simulation started.\n";
    }
}

void Controller::pause()
{
    if (state == SimulationState::RUNNING) {
        state = SimulationState::PAUSED;
        std::cout << "Simulation paused.\n";
    }
}

void Controller::stop()
{
    state = SimulationState::STOPPED;
    map = MapSystem();      // reset grid og feromoner
    colony = AntSystemDOD();   // reset myrer
    nestSet = false;
    ressourceSet = false;
    time = 0.0f;

    std::cout << "Simulation stopped and reset.\n";
}

void Controller::updateFrame()
{
    if (state != SimulationState::RUNNING)
        return;

    updateFrameInternal();
    time += 1.0f; // evt. delta time senere
}

void Controller::updateFrameInternal()
{
    colony.update(map); // myrer bevæger sig
    map.updateAll();       // evaporation
}
