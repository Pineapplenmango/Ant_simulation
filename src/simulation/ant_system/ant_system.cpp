#include "ant_system.h"

AntSystem::AntSystem(/* args */)
{
}

AntSystem::~AntSystem()
{
}



    void AntSystem::spawnAnt(int x, int y)
    {
    if(ants.size() < 3200){
    ants.emplace_back(x,y); 

    }
    else{    

            }

    }

    void AntSystem::removeAnt(){

    if (!ants.empty())
    {
        ants.pop_back();
    }

    }
    void AntSystem::updateAll(MapSystem& map, float time){

          for (auto& ant : ants)
        {
        ant.update(map);
        }

    }