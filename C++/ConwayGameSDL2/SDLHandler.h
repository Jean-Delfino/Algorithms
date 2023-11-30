#ifndef SDLHANDLER_H
#define SDLHANDLER_H

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

class SDLHandler
{
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;

    public:
        SDLHandler(){}

        void Init(string name){
            window = SDL_CreateWindow(name.c_str(), 
                SDL_WINDOWPOS_CENTERED, 
                SDL_WINDOWPOS_CENTERED, 
                800, 
                800, 
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

            renderer = SDL_CreateRenderer(window, 
                -1, 
                SDL_RENDERER_PRESENTVSYNC);
        }

        void Quit(){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
        
        void PollEvent(SDL_Event *ev){
            SDL_PollEvent(ev);
        }

        void SetRenderDrawColor(int x, int y, int z, int a){
            SDL_SetRenderDrawColor(renderer, x, y, z, a);
        }

        void FillRect(SDL_FRect *rect){
            SDL_RenderFillRectF(renderer, rect);
        }

        void GetWindowSize(int *width, int *heigth){
            SDL_GetWindowSize(window, width, heigth);
        }

        void PresentRender(){
            SDL_RenderPresent(renderer);
        }

        void DrawBasicBackground(){
            SDL_SetRenderDrawColor(renderer, 100,100,100,255);
            SDL_RenderClear(renderer);
        }

        void SetWindowName(string name){
            SDL_SetWindowTitle(window, name.c_str());
        }
};



#endif