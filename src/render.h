#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"

class SnekRenderer
{
public:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *background_sur;
  SDL_Texture *background_tex;
  SDL_Rect background_drawrect;

  SnekGame *game;

  SnekRenderer();

private:
  void drawBackground();
  void drawSnek();
  void quit();
};