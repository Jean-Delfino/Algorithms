#include "ConwayGame.h"
#include "SDLEventHandler.h"

static Uint32 nextTime;

Uint32 TimeLeft(){
    Uint32 curTime = SDL_GetTicks();

    return max(nextTime - curTime, (Uint32) 0);
}

void ConwayGame::InitGrid(int cols, int rows){
    int i, j;
    grid.cells.clear();
    
    grid.cols = cols;
    grid.rows = rows;

    for (i = 0; i < rows; i++)
    {
        vector<Cell> cellsCollum;
        for (j = 0; j < cols; j++)
        {
            cellsCollum.push_back(Cell());
        }
        grid.cells.push_back(cellsCollum);
    }
}

void ConwayGame::Draw(){
    bool find = false;
    for (int i = 0; i < grid.rows; i++)
    {
        for (int j = 0; j < grid.cols; j++)
        {
            SDL_FRect rect = {
                grid.cellWidthGap + (float)grid.posX + ((float)j * grid.cellWidth),
                grid.cellHeightGap + (float)grid.posX + ((float)i * grid.cellHeight),
                (float)grid.cellWidth,
                (float)grid.cellHeight,
            };

            if(grid.cells[i][j].alive){
                handler.SetRenderDrawColor(grid.aliveColor.x, 
                    grid.aliveColor.y, 
                    grid.aliveColor.z, 
                    grid.aliveColor.w);

            }else{
                handler.SetRenderDrawColor(grid.deadColor.x, 
                    grid.deadColor.y, 
                    grid.deadColor.z, 
                    grid.deadColor.w);
            }

            if(simulation.mouseX > rect.x && simulation.mouseX < rect.x + rect.w){
                if(simulation.mouseY > rect.y && simulation.mouseY < rect.y + rect.h){
                    simulation.mouseCellX = i;
                    simulation.mouseCellY = j;
                    find = true;

                    handler.SetRenderDrawColor(grid.passingBy.x, 
                        grid.passingBy.y, 
                        grid.passingBy.z, 
                        grid.passingBy.w);
                }
            }

            handler.FillRect(&rect);
        }
    }

    if(!find){
        simulation.mouseCellX = -1;
        simulation.mouseCellY = -1;  
    }

    handler.SetRenderDrawColor(grid.lineColor.x, 
                        grid.lineColor.y, 
                        grid.lineColor.z, 
                        grid.lineColor.w);

	for (int i = 0; i <= grid.rows; i++) {
		SDL_FRect vLine = {
			(float) (grid.posX + (i * grid.cellWidth) + 1),
			(float) (grid.posY + 1),
			2.0f,
			(float) ((i == grid.rows) ? grid.height + 2 : grid.height)
		};

        handler.FillRect(&vLine);
	}
	for (int i = 0; i <= grid.cols; i++) {
		SDL_FRect hLine = {
			(float) (grid.posX + 1),
			(float) (grid.posY + (i * grid.cellHeight) + 1),
			(float) ((i == grid.cols) ? grid.width + 2 : grid.width),
			2.0f
		};
		handler.FillRect(&hLine);
	}
}

void ConwayGame::AdjustGrid(){
    int windowWidth = 0;
    int windowHeight = 0;

    handler.GetWindowSize(&windowWidth, &windowHeight);

    int maxLength = min(windowHeight, windowWidth);

    grid.posX = grid.posY = grid.borderSize;

    int botRight = maxLength - grid.borderSize;

    grid.width = botRight - grid.posX;
    grid.height = botRight - grid.posY;

    grid.cellWidth = (float) grid.width / (float) grid.rows;
    grid.cellHeight = (float) grid.height / (float) grid.cols;

    if(windowWidth > windowHeight){
        grid.posX = ((float) windowWidth / 2.0f) - ((float)grid.width / 2.0f); 
    }else{
        grid.posY = ((float) windowHeight / 2.0f) - ((float)grid.height / 2.0f); 
    }
}

bool CanAcessNeighbour(int maxRows, int maxCols, int row, int col){
    return (row > -1) && (col > -1) && (row < maxRows) && (col < maxCols); 
}   

int GetAllAliveNeighbours(Grid grid, int row, int col) {
  int aliveNeighbours = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int actualRow = row + i;
      int actualCol = col + j;

      if (i == 0 && j == 0) continue;

      if (CanAcessNeighbour(grid.rows, grid.cols, actualRow, actualCol) && 
            grid.cells[actualRow][actualCol].alive){
        aliveNeighbours++;
      }
    }
  }

  return aliveNeighbours;
}

bool ConWayCellAliveState(int neighbours){
    if(neighbours == 3 || neighbours == 2) return true;
    
    return false;
}

void ConwayGame::RunSimulation(){
    for (int i = 0; i < grid.rows; i++)
    {
        for (int j = 0; j < grid.cols; j++)
        {
            grid.cells[i][j].neightbours = GetAllAliveNeighbours(grid, i, j);
        } 
    }

    while (simulation.simTick < simulation.simDelay)
    {
        SDL_Delay(1);
        simulation.simTick++;
    }
    
    for (int i = 0; i < grid.rows; i++)
    {
        for (int j = 0; j < grid.cols; j++)
        {
            grid.cells[i][j].SetAliveState(ConWayCellAliveState(grid.cells[i][j].neightbours));
        }
    }
    
    simulation.simTick = 0;
}

void ConwayGame::HandleEvents(SDL_Event ev){
    handler.PollEvent(&ev);
        
    if (SDLEventHandler::isQuitEvent(ev) || SDLEventHandler::windowClose(ev)) {
        isRunning = false;
        simulation.isRunning = false;
        return;
    }

    if(SDLEventHandler::isButtonDown(ev) && 
            SDLEventHandler::isLeftMouseButton(ev)){
        simulation.mouseDown = true;
    }else if(SDLEventHandler::isButtonUp(ev) && 
            SDLEventHandler::isLeftMouseButton(ev)){
        simulation.mouseDown = false;   
    }

    if(SDLEventHandler::isMouseMotion(ev)){
        SDL_GetMouseState(&simulation.mouseX, &simulation.mouseY);
    }
    
    if(SDLEventHandler::windowChanged(ev)){
        AdjustGrid();
    }

    if(SDLEventHandler::isSpaceEvent(ev)){
        simulation.isRunning = !simulation.isRunning;
        //Change name of the screen logic
        if(simulation.isRunning) handler.SetWindowName(RUNNING_PHRASE);
        else handler.SetWindowName(STOPPED_PHRASE);
    }
}

void ConwayGame::CheckClick(){
    if(simulation.mouseDown){
        if(simulation.mouseCellX > -1 && simulation.mouseCellY > -1){
            grid.cells[simulation.mouseCellX][simulation.mouseCellY].SwitchLife();
        }
    }
}

void ConwayGame::Run(){
    SDL_Event ev;

    while (isRunning)
    {
        handler.DrawBasicBackground();

        Draw();
        handler.PresentRender();

        HandleEvents(ev);

        if(simulation.isRunning && !simulation.mouseDown) RunSimulation();

        SDL_Delay(TimeLeft());
        nextTime += TICK_INTERVAL;
    }

    handler.Quit();
}

ConwayGame::ConwayGame() {
    Init();
    InitGrid(20, 20);

    nextTime = SDL_GetTicks() + TICK_INTERVAL;
}

void ConwayGame::Init(){
    handler.Init(STARTER_PHRASE);
    isRunning = true;
}

