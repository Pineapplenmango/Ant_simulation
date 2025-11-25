#pragma once
#include <vector>
#include "../cell/cell.h"
using namespace std;


/**
 * @class MapSystem
 * @brief Sørger for alt bane relateret logik kan tilgåes af controller
 *
 * 
 * 
 */



class MapSystem
{
private:
    /* data */
    std::vector<std::vector<Cell>> cellGrid; // [x][y] grid

    std::vector<Cell> cellGridDod;
    
    pair<int,int> nestPoint;
    pair<int,int> resourcePoint;
    void evaporateAll();
    

public:
    MapSystem(/* args */);
    ~MapSystem();

    void setObstacle(int x, int y);
    void setNestPoint(int x, int y);
    void setRessourcePoint(int x, int y);
    void addPheromone(int x, int y, float amount);

    std::vector<std::pair<int,int>> findNeighbors(int x, int y); //omdøbt fra get til find da det ikke er en get-funktion


    void updateAll();



    int getNestX() const { return nestPoint.first; }
    int getNestY() const { return nestPoint.second; }
    int getResourceX() const { return resourcePoint.first; }
    int getResourceY() const { return resourcePoint.second; }

    int getHeight() const { return cellGrid.size(); }
    int getWidth() const { return cellGrid.empty() ? 0 : cellGrid[0].size(); }   
    int ressourcesDelivered = 0;

    Cell& getCell(int x, int y){return cellGrid[y][x];}
    const Cell& getCell(int x, int y) const {return cellGrid[y][x];}



};


