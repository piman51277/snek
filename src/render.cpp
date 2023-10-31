#include "render.h"
#include <deque>
#include <cinttypes>
#include <iostream>

SnekRenderer::SnekRenderer()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);

  window = SDL_CreateWindow("snek", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 884, 884, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  background_sur = IMG_Load("img/background-lg.png");
  background_tex = SDL_CreateTextureFromSurface(renderer, background_sur);
  background_drawrect = {0, 0, 884, 884};
  game = new SnekGame();

  SDL_TimerCallback renderCallback = [](uint32_t interval, void *param) -> uint32_t
  {
    SnekRenderer *renderer = (SnekRenderer *)param;

    SnekGame *game = renderer->game;
    game->nextTick();

    if (game->game_over)
    {
      if (game->score == 400)
      {
        std::cout << "Won in " << game->ticks << " ticks!" << std::endl;
      }
      else
      {
        std::cout << "Lost in " << game->ticks << " ticks!" << std::endl;
      }
      game->reset();
    }

    renderer->drawBackground();
    renderer->drawSnek();
    SDL_RenderPresent(renderer->renderer);
    return interval;
  };

  SDL_AddTimer(1000 / 60, renderCallback, this);

  while (true)
  {
    SDL_Event e;
    if (SDL_WaitEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        quit();
        break;
      }
    }
  }
}

void SnekRenderer::drawBackground()
{
  SDL_RenderCopy(renderer, background_tex, NULL, &background_drawrect);
}

void SnekRenderer::drawSnek()
{
  Snek snek = game->snek;
  std::deque<uint16_t> x = snek.x;

  const int thickness = 18;
  const int cellSize = 44;
  const int cellOffset = 24;

  // special case for length 1
  if (x.size() == 1)
  {
    int x1 = (x[0] % 20) * cellSize + cellOffset;
    int y1 = (x[0] / 20) * cellSize + cellOffset;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_Rect rect = {x1 - thickness / 2, y1 - thickness / 2, thickness, thickness};
    SDL_RenderFillRect(renderer, &rect);
  }
  else
  {
    // for every pair of points, draw a "line" between them

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < x.size() - 1; i++)
    {

      // convert i to x and y
      int x1 = (x[i] % 20) * cellSize + cellOffset;
      int y1 = (x[i] / 20) * cellSize + cellOffset;

      int x2 = (x[i + 1] % 20) * cellSize + cellOffset;
      int y2 = (x[i + 1] / 20) * cellSize + cellOffset;

      // if the line is vertical
      if (x1 == x2)
      {
        int y_min = std::min(y1, y2);
        SDL_Rect rect = {x1 - thickness / 2, y_min - thickness / 2, thickness, std::abs(y2 - y1) + thickness};
        SDL_RenderFillRect(renderer, &rect);
      }
      else
      {
        int x_min = std::min(x1, x2);
        SDL_Rect rect = {x_min - thickness / 2, y1 - thickness / 2, std::abs(x2 - x1) + thickness, thickness};
        SDL_RenderFillRect(renderer, &rect);
      }
    }

    // make the head blue
    int x1 = (x.back() % 20) * cellSize + cellOffset;
    int y1 = (x.back() / 20) * cellSize + cellOffset;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect = {x1 - thickness / 2, y1 - thickness / 2, thickness, thickness};
    SDL_RenderFillRect(renderer, &rect);
  }

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  // draw food

  int x1 = (game->food % 20) * cellSize + cellOffset;
  int y1 = (game->food / 20) * cellSize + cellOffset;

  SDL_Rect rect = {x1 - thickness / 2, y1 - thickness / 2, thickness, thickness};
  SDL_RenderFillRect(renderer, &rect);
}

void SnekRenderer::quit()
{
  SDL_DestroyTexture(background_tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
}