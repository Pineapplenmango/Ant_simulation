#include "ant_system_dod.h"
#include <cmath>

AntSystemDOD::AntSystemDOD()
{
}

AntSystemDOD::~AntSystemDOD()
{
}


void AntSystemDOD::spawnAnt(int x, int y)
{
    positionX.push_back(x);
    positionY.push_back(y);
    carryingRessource.push_back(false);
    alpha.push_back(2.0f);
    beta.push_back(0.5f);
    path.emplace_back(); // tom sti til denne myre
}

void AntSystemDOD::removeAnt()
{
    if (positionX.empty()) return;

    positionX.pop_back();
    positionY.pop_back();
    carryingRessource.pop_back();
    alpha.pop_back();
    beta.pop_back();
    path.pop_back();
}





void AntSystemDOD::update(MapSystem& map)
{
      const int count = (int)positionX.size();
    for (int i = 0; i < count; ++i)
    {
        updateAnt(i, map);  
    }
}


void AntSystemDOD::updateAnt(int i, MapSystem& map)
{
    

    if (path[i].size() > 1000) {
        carryingRessource[i] = false;
        positionX[i] = map.getNestX();
        positionY[i] = map.getNestY();
        path[i].clear();
    }

    // Hvis den leder efter ressource
    if (!carryingRessource[i])
    {
        if (!carryingRessource[i] &&
            positionX[i] == map.getResourceX() &&
            positionY[i] == map.getResourceY())
        {
            carryingRessource[i] = true;
            return;
        }

        move(i, map);
    }
    // hvis den har fundet ressource
    else
    {
        float distToNest = std::hypot(
            positionX[i] - map.getNestX(),
            positionY[i] - map.getNestY()
        );

        float k     = 0.08f;   // hvor hurtigt feromonet vokser med afstand
        float base  = 0.02f;   // maksimal afsætning i den fjerne ende
        float strength = base * (1.0f - std::exp(-k * distToNest));

        map.addPheromone(positionX[i], positionY[i], strength);

        if (!path[i].empty()) {
            auto [px, py] = path[i].back();
            path[i].pop_back();
            positionX[i] = px;
            positionY[i] = py;
        } else {
            if (std::abs(positionX[i] - map.getNestX()) <= 1 &&
                std::abs(positionY[i] - map.getNestY()) <= 1)
            {
                map.ressourcesDelivered++;
                carryingRessource[i] = false;
            }
            else
            {
                // Mistet vej hjem, drop feromon og vend tilbage til søgning
                carryingRessource[i] = false;
                path[i].clear();
            }
        }
    }
}



void AntSystemDOD::move(int i, MapSystem& map)
{
    auto [nextX, nextY] = chooseNextCell(i, map);

    // gem nuværende position i sti
    path[i].push_back({ positionX[i], positionY[i] });

    positionX[i] = nextX;
    positionY[i] = nextY;
}





std::pair<int,int> AntSystemDOD::chooseNextCell(int i, MapSystem& map)
{
    const int x = positionX[i];
    const int y = positionY[i];

    
    int nxs[4];
    int nys[4];
    int neighborCount = 0;

    auto tryAdd = [&](int nx, int ny)
    {
        if (nx < 0 || nx >= map.getWidth())  return;
        if (ny < 0 || ny >= map.getHeight()) return;
        if (!map.getCell(nx, ny).isWalkable()) return;

        nxs[neighborCount] = nx;
        nys[neighborCount] = ny;
        ++neighborCount;
    };

    tryAdd(x + 1, y);
    tryAdd(x - 1, y);
    tryAdd(x, y + 1);
    tryAdd(x, y - 1);

    if (neighborCount == 0)
        return { x, y };

   
    const int memoryLength = 6;
    bool valid[4] = { true, true, true, true };

    int startIdx = std::max(0, (int)path[i].size() - memoryLength);
    for (int h = startIdx; h < (int)path[i].size(); ++h)
    {
        auto [hx, hy] = path[i][h];
        for (int j = 0; j < neighborCount; ++j)
        {
            if (valid[j] && nxs[j] == hx && nys[j] == hy)
                valid[j] = false;
        }
    }

   
    int write = 0;
    for (int j = 0; j < neighborCount; ++j)
    {
        if (valid[j])
        {
            nxs[write] = nxs[j];
            nys[write] = nys[j];
            ++write;
        }
    }
    neighborCount = write;

    
    if (neighborCount == 0)
    {
        neighborCount = 0;
        tryAdd(x + 1, y);
        tryAdd(x - 1, y);
        tryAdd(x, y + 1);
        tryAdd(x, y - 1);
        if (neighborCount == 0)
            return { x, y };
    }

    
    float dirx = 0.0f, diry = 0.0f;
    if (!path[i].empty()) {
        dirx = (float)(x - path[i].back().first);
        diry = (float)(y - path[i].back().second);
        float len = std::sqrt(dirx * dirx + diry * diry);
        if (len > 0.0f) { dirx /= len; diry /= len; }
    }

    const float directionBiasStrength = 0.7f;

    // 4) Dorigo-vægtning + retningsbias
    float weights[4];
    float total = 0.0f;

    for (int j = 0; j < neighborCount; ++j)
    {
        int nx = nxs[j];
        int ny = nys[j];

        float tau = map.getCell(nx, ny).getPheromone();
        if (tau < 1e-6f) tau = 1e-6f;

        
        float stepx = (float)(nx - x);
        float stepy = (float)(ny - y);
        float slen  = std::sqrt(stepx*stepx + stepy*stepy);
        if (slen > 0.0f) { stepx /= slen; stepy /= slen; }
        float cosTheta = stepx * dirx + stepy * diry;
        float bias = 1.0f + directionBiasStrength * cosTheta;
        if (bias < 0.1f) bias = 0.1f;

        // samme alpha/beta logik
        float a = alpha[i];
        float b = beta[i];
        float tauInfluence = std::pow(tau, a / b);

        float noise = 0.8f + ((float)rand() / RAND_MAX) * (0.4f / b);

        float value = tauInfluence * bias * noise;
        weights[j] = value;
        total += value;
    }

    if (total <= 0.0f)
    {
        int rIndex = rand() % neighborCount;
        return { nxs[rIndex], nys[rIndex] };
    }

    float r = static_cast<float>(rand()) / RAND_MAX;
    float cumulative = 0.0f;

    for (int j = 0; j < neighborCount; ++j)
    {
        cumulative += weights[j] / total;
        if (r <= cumulative)
            return { nxs[j], nys[j] };
    }

    return { nxs[neighborCount - 1], nys[neighborCount - 1] };
}