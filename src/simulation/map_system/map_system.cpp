#include "map_system.h"

MapSystem::MapSystem()
{
    cellGrid.resize(HEIGHT, std::vector<Cell>(WIDTH));


      for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
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
    if (!inBounds(x, y)) return;
    cellGrid[y][x].setWalkable(false);
}


bool MapSystem::setNestPoint(int x, int y)
{
    if(!inBounds(x,y)) return false;
    nestPoint= {x,y}; 
    return true;
}

bool MapSystem::setRessourcePoint(int x, int y)
{
    if(!inBounds(x,y)) return false;
    resourcePoint = {x,y};
    return true;
}

void MapSystem::addPheromone(int x, int y, float amount)
{

    if (amount <= 0.0f) return;

    Cell& c = getCell(x, y);
    float p = c.getPheromone();

    // Brug samme cap som i Cell::setPheromone
    const float MAX = 10.0f; // hold dette identisk med cap i setPheromone

    // "Soft saturation": jo tættere på MAX, desto mindre ekstra effekt
    // p_new = p + amount * (1 - p/MAX)
    float newP = p + amount * (1.0f - p / MAX);

    c.setPheromone(newP); // setPheromone sørger stadig for clamp [0, MAX]



}



std::vector<std::pair<int,int>> MapSystem::findNeighbors(int x, int y)
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

     const float evaporationRate = 0.025f; // justér efter behov (1% pr. frame fx)

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

