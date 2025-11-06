#include "ant.h"
#include <cmath>
#include <algorithm>


Ant::Ant(int startX, int StartY) : positionX(startX), positionY(StartY), carryingRessource(false), alpha(3.0f), beta(1.5f)
{
    path.clear();
}

Ant::~Ant()
{
}

void Ant::update(MapSystem& map)
{


  



    if(path.size()>400){

                positionX = map.getNestX();
                 positionY = map.getNestY();
                carryingRessource = false;
                beta = 1.5f; // samme som init
                path.clear();
              
            
        }


   // if(path.size()>300){beta = 5.0f;}



     //Hvis den leder efter ressource
    if (!carryingRessource)
    {
        // tjek om position er ressource
        /*if (positionX == map.getResourceX() && positionY == map.getResourceY()){
          
            if (!carryingRessource && positionX == map.getResourceX() && positionY == map.getResourceY())
                std::cout << "FOUND resource at " << positionX << "," << positionY << std::endl;

             carryingRessource = true;

            //setPath = path; //konstant path i memory
            return; // begynder at backtracke i næste frame
        }*/

        if (!carryingRessource &&
                positionX == map.getResourceX() &&
                 positionY == map.getResourceY())
                {
                    carryingRessource = true;
                    std::cout << "FOUND resource at " << positionX << "," << positionY << std::endl;
                        return;
                        }




        move(map);

        // tjek igen om position er ressource
        /*if (positionX == map.getResourceX() && positionY == map.getResourceY()){
       
                if (!carryingRessource && positionX == map.getResourceX() && positionY == map.getResourceY())
                std::cout << "FOUND resource at " << positionX << "," << positionY << std::endl;
                    carryingRessource = true;


            //setPath = path; //konstant path i memory
            return;
        }*/
                        //er det hre virkelig nødvendigt..? tror ikke
           /*     if (!carryingRessource &&
                positionX == map.getResourceX() &&
                 positionY == map.getResourceY())
                {
                    carryingRessource = true;
                    std::cout << "FOUND resource at " << positionX << "," << positionY << std::endl;
                        return;
                        }*/





    }
    // hvis den har fundet ressource
    else
    {


       // static int frameCounter = 0;
       // if (frameCounter++ % 10 == 0) // kun hvert 10. frame
      //      map.addPheromone(positionX, positionY, 0.05f);
        
        //map.addPheromone(positionX, positionY, 1.0f);
       // if (!(positionX == map.getNestX() && positionY == map.getNestY()))
                //map.addPheromone(positionX, positionY, 0.005f);


              //  float distToNest = std::hypot(positionX - map.getNestX(), positionY - map.getNestY());
              //  float strength = 0.01f * std::exp(-distToNest * 0.02f);
               // map.addPheromone(positionX, positionY, strength);

                float distToNest = std::hypot(positionX - map.getNestX(), positionY - map.getNestY());
                float k = 0.03f;             // hvor hurtigt feromonet vokser med afstand
                float base = 0.025f;          // maksimal afsætning i den fjerne ende
                float strength = base * (1.0f - std::exp(-k * distToNest));
                map.addPheromone(positionX, positionY, strength);



        /*if (!path.empty()){
            auto [px,py] = path.back(); path.pop_back();
            positionX = px; positionY = py;
        } 
        else 
        {
            // tjek om position er nest
          //  if (positionX == map.getNestX() && positionY == map.getNestY()){
            //    carryingRessource = false; // ny tur
           // }
             if (std::abs(positionX - map.getNestX()) <= 1 && std::abs(positionY - map.getNestY()) <= 1)
                {
                    carryingRessource = false;
                    path.clear();
                }
        }*/

        

        if (!path.empty()) {
            auto [px, py] = path.back(); path.pop_back();
         positionX = px; positionY = py;
        } else {
    if (std::abs(positionX - map.getNestX()) <= 1 &&
        std::abs(positionY - map.getNestY()) <= 1) {
            std::cout << "ressource returned at nest at " << positionX << "," << positionY << std::endl;
        carryingRessource = false;
            } else {
        // Mistet vej hjem, drop feromon og vend tilbage til søgning
        std::cout << "resource lost " << std::endl;
        carryingRessource = false;
        path.clear();
            }
        }




    }

}


std::pair<int,int> Ant::chooseNextCell(MapSystem& map)
{
    auto neighbors = map.getNeighbors(positionX, positionY);
    if (neighbors.empty()) return {positionX, positionY};


    // 1️⃣ Fjern de seneste 4 besøgte celler (korttidshukommelse)
    const int memoryLength = 6; 
    int startIdx = std::max(0, (int)path.size() - memoryLength);

    for (int i = startIdx; i < (int)path.size(); ++i)
    {
        auto [hx, hy] = path[i];
        neighbors.erase(
            std::remove_if(neighbors.begin(), neighbors.end(),
                           [&](const std::pair<int,int>& n)
                           { return n.first == hx && n.second == hy; }),
            neighbors.end());
    }

    // Hvis alle naboer er filtreret væk, vælg igen blandt alle (for at undgå fastlåsning)
    if (neighbors.empty())
        neighbors = map.getNeighbors(positionX, positionY);

    // 2️⃣ Beregn retningsbias (retningsstabilitet)
    float dx_prev = 0.0f;
    float dy_prev = 0.0f;
    if (!path.empty())
    {
        dx_prev = (float)(positionX - path.back().first);
        dy_prev = (float)(positionY - path.back().second);
    }

    // Bias-styrke: hvor meget retningsstabilitet skal påvirke bevægelsen
    const float directionBiasStrength = 1.5f; 



    

    //dorigo equation..?
    std::vector<float> weights;
    float total = 0.0f;

    for (auto [nx, ny] : neighbors)
    {
        float tau = map.getCell(nx, ny).getPheromone();


        // 🐜 Hvis myren IKKE bærer på ressource, skal den ikke være så tiltrukket af feromon
       //  if (!carryingRessource) {
       // tau *= 0.5f; // reducer feromonets betydning kraftigt (kan tunes: 0.01f–0.2f)
         //   }


        float eta = 0.5f;
        float value = pow(tau, alpha) * pow(eta, beta);
        weights.push_back(value);
        total += value;
    }

    if (total <= 0.0f)
        return neighbors[rand() % neighbors.size()];

    float r = static_cast<float>(rand()) / RAND_MAX;
    float cumulative = 0.0f;
    for (size_t i = 0; i < neighbors.size(); ++i)
    {
        cumulative += weights[i] / total;
        if (r <= cumulative)
            return neighbors[i];
    }

    return neighbors.back();
}


void Ant::move(MapSystem& map)
{
    auto [nextX, nextY] = chooseNextCell(map);
    path.push_back({positionX, positionY});
    positionX = nextX;
    positionY = nextY;
}




