#pragma once
#include <vector>
#include "../cell/cell.h"


/**
 * @class MapSystem
 * @brief Ansvarlig for simulationens grid, feromoner og faste punkter.
 *
 * MapSystem repræsenterer miljøet, som myrerne bevæger sig i.
 * Det indeholder:
 *  - Et 2D-grid af Cell-objekter
 *  - Placering af rede og ressource
 *  - Logik til feromon-afsætning og fordampning
 *
 * Systemet er adskilt fra myrelogik for at overholde Single Responsibility.
 */


class MapSystem
{
private:
   
     /// 2D grid af celler repræsenterende banen ([x][y] grid)
    std::vector<std::vector<Cell>> cellGrid;

    //std::vector<Cell> cellGridDod;
    
    /// Position for redepunktet
    std::pair<int,int> nestPoint;

     /// Position for ressourcepunktet
    std::pair<int,int> resourcePoint;
   
public:
    
    MapSystem();
    ~MapSystem();


    
    static constexpr int HEIGHT =80;
    static constexpr int WIDTH =80;



    /**
     * @brief Marker en celle som ikke-gåbar.
     */
    void setObstacle(int x, int y);

    /**
     * @brief Sætter koordinater for reden.
     */
    bool setNestPoint(int x, int y);


    /**
     * @brief Sætter koordinater for ressourcen.
     */
    bool setRessourcePoint(int x, int y);

    /**
     * @brief Tilføjer feromon til en bestemt celle.
     *
     * Bruges af myrer, når de returnerer til reden.
     * Der anvendes "soft saturation" for at undgå ekstreme værdier.
     */
    void addPheromone(int x, int y, float amount);


    /**
     * @brief Finder alle gyldige naboceller (4-vejs).
     *
     * @return Liste af koordinater der kan bevæges til.
     */
    std::vector<std::pair<int,int>> findNeighbors(int x, int y); 


    /**
     * @brief Opdaterer hele gridet (feromon-fordampning).
     */
    void updateAll();


    /// Getters til rede og ressource
    int getNestX() const { return nestPoint.first; }
    int getNestY() const { return nestPoint.second; }
    int getResourceX() const { return resourcePoint.first; }
    int getResourceY() const { return resourcePoint.second; }


     /// Dimensioner
    int getHeight() const { return cellGrid.size(); }
    int getWidth() const { return cellGrid.empty() ? 0 : cellGrid[0].size(); }   

      /// Statistik: leverede ressourcer
    int ressourcesDelivered = 0;

    /// Direkte celle-adgang (bruges af systemer)
    Cell& getCell(int x, int y){return cellGrid[y][x];}
    const Cell& getCell(int x, int y) const {return cellGrid[y][x];}


    bool inBounds(int x, int y) const { return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT; }



};


