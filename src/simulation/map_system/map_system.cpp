#include "map_system.h"

MapSystem::MapSystem()
{

    const int width = 80;   // strl af map, kan justeres
    const int height = 80;

    cellGrid.resize(height, std::vector<Cell>(width));


      for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            cellGrid[y][x].setWalkable(true);
            cellGrid[y][x].setPheromone(0.0f);
        }
    }


    nestPoint = {-1,-1}; // ikke defineret endnu, når mapsystem constructes
    resourcePoint = {-1,-1};
}

MapSystem::~MapSystem()
{
}

void MapSystem::setObstacle(int x, int y)
{
    if (y >= 0 && y < (int)cellGrid.size() &&
        x >= 0 && x < (int)cellGrid[0].size())
        {
            cellGrid[y][x].setWalkable(false);
        }
    
}


void MapSystem::setNestPoint(int x, int y)
{
 nestPoint= {x,y};

}

void MapSystem::setRessourcePoint(int x, int y)
{

    resourcePoint = {x,y};
}

void MapSystem::addPheromone(int x, int y, float amount)
{
    if (y >= 0 && y < (int)cellGrid.size() &&
        x >= 0 && x < (int)cellGrid[0].size())
    {
        float v = cellGrid[y][x].getPheromone() + amount;
        cellGrid[y][x].setPheromone(v);
    }

}



std::vector<std::pair<int,int>> MapSystem::getNeighbors(int x, int y)
{
    std::vector<std::pair<int,int>> result;
    static const int dirs[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    for (auto& d : dirs)
    {
        int nx = x + d[0];
        int ny = y + d[1];

        if (ny >= 0 && ny < (int)cellGrid.size() &&
            nx >= 0 && nx < (int)cellGrid[0].size() &&
            cellGrid[ny][nx].isWalkable())
        {
            result.push_back({nx, ny});
        }
    }
    return result;
}




void MapSystem::updateAll()
{

     const float evaporationRate = 0.001f; // justér efter behov (1% pr. frame fx)

    for (auto& row : cellGrid)
    {
        for (auto& cell : row)
        {
            if (cell.getPheromone() > 0.0f)
            {
                float newPhero = cell.getPheromone() * (1.0f - evaporationRate);
                if (newPhero < 0.0001f) newPhero = 0.0f; // undgå  rester
                cell.setPheromone(newPhero);
            }
        }
    }
}