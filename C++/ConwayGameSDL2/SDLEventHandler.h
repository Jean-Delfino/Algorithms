#ifndef SDLEVENTHANDLER_H
#define SDLEVENTHANDLER_H

#include <SDL2/SDL.h>

namespace SDLEventHandler {
    bool isQuitEvent(SDL_Event ev){
        return ev.type == SDL_QUIT;
    }

    bool isSpaceEvent(SDL_Event ev){
        return ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_SPACE;
    }

    bool windowChanged(SDL_Event ev){
        return ev.type == SDL_WINDOWEVENT && 
                (ev.window.event == SDL_WINDOW_FULLSCREEN || 
                ev.window.event == SDL_WINDOWEVENT_RESIZED || 
                ev.window.event == SDL_WINDOWEVENT_RESTORED);
    }

    bool windowClose(SDL_Event ev){
        return ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE;
    }

    bool isMouseMotion(SDL_Event ev){
        return ev.type == SDL_MOUSEMOTION;
    }

    bool isButtonDown(SDL_Event ev){
        return ev.type == SDL_JOYBUTTONDOWN;
    }

    bool isButtonUp(SDL_Event ev){
        return ev.type == SDL_MOUSEBUTTONUP;
    }

    bool isLeftMouseButton(SDL_Event ev){
        return ev.button.button == SDL_BUTTON_LEFT;
    }
}


#endif