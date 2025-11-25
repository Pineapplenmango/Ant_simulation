#include "raylib.h"
//#include "controller.h"
#include "simulation/controller/controller.h"


//terminal kommandoer
/*
build projekt med: g++ -std=c++17 -o ant_simulation.exe src/main.cpp src/simulation/ant/*.cpp src/simulation/ant_system/*.cpp 
src/simulation/map_system/*.cpp src/simulation/controller/*.cpp src/simulation/cell/*.cpp
-Isrc/simulation -I C:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
*/
// kør projekt med: ./ant_simulation.exe i terminal



const int CELL_SIZE = 10;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ant Simulation");
    SetTargetFPS(120);

    Controller sim;


    bool placingNest = false;
    bool placingResource = false;
    bool drawingObstacle = false;

    while (!WindowShouldClose())
    {

        //   event handling
 

        if (IsKeyPressed(KEY_SPACE))
            sim.start();

        if (IsKeyPressed(KEY_P))
            sim.pause();

        if (IsKeyPressed(KEY_R))
            sim.stop();

        // Aktiver “placer rede” tilstand
        if (IsKeyPressed(KEY_N)) {
            placingNest = true;
            placingResource = false;
            drawingObstacle = false;
        }

        // Aktiver “placer ressource” tilstand
        if (IsKeyPressed(KEY_M)) {
            placingResource = true;
            placingNest = false;
            drawingObstacle = false;
        }

        // Aktivér obstacle-tilstand 
        if (IsKeyPressed(KEY_O)) {
            drawingObstacle = !drawingObstacle;
            placingNest = false;
            placingResource = false;
        }

        // klik for specifikt event
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 pos = GetMousePosition();
            int x = pos.x / CELL_SIZE;
            int y = pos.y / CELL_SIZE;

            if (placingNest) {
                sim.placeNestPoint(x, y);
                placingNest = false;
            } 
            else if (placingResource) {
                sim.placeRessourcePoint(x, y);
                placingResource = false;
            } 
            else if (drawingObstacle) {
                sim.drawObstacles({{x, y}});
            }
        }



        if (IsKeyPressed(KEY_K))
            sim.addAnts(100);   // tilføj 100 ad gangen




        if (IsKeyPressed(KEY_A))
            sim.addAnts(1);   // tilføj én ad gangen




   
        if (IsKeyPressed(KEY_D))
            sim.removeAnts(1);   // fjern én ad gangen


        if (IsKeyPressed(KEY_L))
            sim.removeAnts(100);   // fjern 100 ad gangen


        //    opdater positioner




        
        sim.updateFrame();


        //     drawing phase


        BeginDrawing();
        ClearBackground(RAYWHITE);

        const MapSystem& map = sim.getMap();

        for (int y = 0; y < map.getHeight(); ++y)
        {
            for (int x = 0; x < map.getWidth(); ++x)
            {
                const Cell& cell = map.getCell(x, y);

                Color color = RAYWHITE;

                if (!cell.isWalkable())
                    color = DARKGRAY;
                else if (cell.getPheromone() > 0.0f)
                {
                    // Lys orange ved stærke feromoner
                    int intensity = (int)(cell.getPheromone() * 255);
                    if (intensity > 255) intensity = 255;
                    color = Color{255, 200, 0, (unsigned char)intensity};
                }

                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
            }
        }

        // Tegn nest og ressourcepunkt
        DrawCircle(map.getNestX() * CELL_SIZE + CELL_SIZE / 2,
                   map.getNestY() * CELL_SIZE + CELL_SIZE / 2,
                   CELL_SIZE / 2, GREEN);

        DrawCircle(map.getResourceX() * CELL_SIZE + CELL_SIZE / 2,
                   map.getResourceY() * CELL_SIZE + CELL_SIZE / 2,
                   CELL_SIZE / 2, BLUE);

        // Tegn myrer
        /*const AntSystem& ants = sim.getColony();
        for (const auto& a : ants.getAnts())
        {
            DrawCircle(a.getX() * CELL_SIZE + CELL_SIZE / 2,
                       a.getY() * CELL_SIZE + CELL_SIZE / 2,
                       CELL_SIZE / 3, RED);
        }*/

        const AntSystemDOD& ants = sim.getColony();
        for (int i = 0; i < ants.getCount(); ++i)
        {
            DrawCircle(
            ants.getX(i) * CELL_SIZE + CELL_SIZE/2,
            ants.getY(i) * CELL_SIZE + CELL_SIZE/2,
            CELL_SIZE / 3,
            RED
            );
        }





        // UI-overlay tekst
        DrawText("SPACE=Start  P=Pause  R=Reset  N=Nest  M=Resource  O=Obstacles  A=Add ant D=Remove ant K/L=Add/remove 100 ants",
                 10, 10, 14, BLACK);
        DrawText(TextFormat("Performance:"), 10, 50, 15, RED);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 70, 15, DARKGRAY);
        DrawText(TextFormat("Behaviour "), 10, 100, 15, RED);
        DrawText(TextFormat("Ant count: %d", ants.getCount()), 10, 120, 15, DARKGRAY);
        DrawText(TextFormat("ressources delivered: %d", map.ressourcesDelivered), 10, 140, 15, DARKGRAY);


        EndDrawing();
    }

    CloseWindow();
    return 0;





}