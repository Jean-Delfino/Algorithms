#ifndef CONWAYGAME_H
#define CONWAYGAME_H

#include "SDLHandler.h"

#include <iostream>
#include <vector>

using namespace std;

#define TICK_INTERVAL 20

#define RUNNING_PHRASE "Conway (Simulation) - Running" 
#define STOPPED_PHRASE "Conway (Simulation) - Stopped"
#define STARTER_PHRASE "Conway (Simulation) - Waiting"

struct Cell
{
    Cell(bool isAlive = false){
        alive = isAlive;
    }

    void Die(){
        alive = false;
    }
    void Born(){
        alive = true;
    }
    void SwitchLife(){
        alive = !alive;
    }
    void SetAliveState(bool state){
        alive = state;
    }

    bool alive = false;
    int neightbours = 0; //alive neightboors
};

struct SimpleVector4
{
    int x;
    int y;
    int z;
    int w;

    SimpleVector4(int x, int y, int z, int w): x(x), y(y), z(z), w(w){}
};


struct Grid
{
    Grid(){}

    int cols = 0;
    int rows = 0;
    
    int posX = 0;
    int posY = 0;

    int width = 0;
    int height = 0;

    int cellWidth = 0;
    int cellHeight = 0;

    float cellWidthGap = 1.05;
    float cellHeightGap = 1.05;

    int borderSize = 20;

    SimpleVector4 passingBy = SimpleVector4(15,15,15,255);
    SimpleVector4 aliveColor = SimpleVector4(20,20,20,255);
    SimpleVector4 deadColor = SimpleVector4(200,200,200,255);

    SimpleVector4 lineColor = SimpleVector4(50,50,50,255);

    vector<vector<Cell>> cells;
};

struct Simulation{
    bool isRunning = false;
    int simTick = 0;
    int simDelay = 40;

    int mouseX = 0;
    int mouseY = 0;

    int mouseCellX = -1;
    int mouseCellY = -1;

    bool mouseDown = false;
};


class ConwayGame
{
    public:
        ConwayGame();

        void Init();
        void Run();
        void RunSimulation();
        void HandleEvents(SDL_Event ev);
        void CheckClick();
        
    private:
        bool isRunning;
        SDLHandler handler;
        Grid grid;
        Simulation simulation;

        void InitGrid(int cols, int rows);
        void Draw();
        void AdjustGrid();
};

#endif