#include "ant_system.h"

AntSystem::AntSystem(/* args */)
{
}

AntSystem::~AntSystem()
{
}



    void AntSystem::spawnAnt(int x, int y)
    {
    if(ants.size() < 100){
    ants.emplace_back(x,y); 
    std::cout << "Ant added at (" << x << ", " << y << ")\n";
    std::cout << "Ant count: " << ants.size() << std::endl;
    }
    else{    
        std::cout << "Ant count: " << ants.size() << std::endl;
        std::cout << "Ants max reached";
            }

    }

    void AntSystem::removeAnt(){

    if (!ants.empty())
    {
        ants.pop_back();
        std::cout << "Ant removed\n";
    }

    }
    void AntSystem::updateAll(MapSystem& map, float time){

          for (auto& ant : ants)
    {
        ant.update(map);
    }

    }