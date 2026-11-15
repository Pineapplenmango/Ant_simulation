#include "ant_system.h"




AntSystem::AntSystem(/* args */)
{
}

AntSystem::~AntSystem()
{
}



    void AntSystem::spawnAnt(int x, int y){ 
        ants.emplace_back(x,y);
    }

    void AntSystem::removeAnt(){

    if (!ants.empty())
    {
        ants.pop_back();
    }

    }
    void AntSystem::updateAll(MapSystem& map){

          for (auto& ant : ants)
        {
        ant.update(map);
        }

    }