#include "render.h"
#include "game.h"

#include <vector>
#include <iostream>
#include <chrono>

int main()
{

  // Comment these lines out for analysis mode
  SnekRenderer renderer;
  return 0;

  SnekGame game;

  int sum = 0;
  int count = 0;
  int wins = 0;
  int fails = 0;
  int trials = 5000;
  while (true)
  {
    game.nextTick();
    if (game.game_over)
    {
      if (game.score == 64)
      {
        sum += game.ticks;
        wins++;
      }
      else
      {
        fails++;
      }
      count++;
      game.reset();

      if (count == trials)
      {
        break;
      }
    }
  }

  float averageWinTicks = (float)sum / (float)wins;
  std::cout << "Average win ticks: " << averageWinTicks << std::endl;

  float failRate = (float)fails / (float)count;
  std::cout << "Fail rate: " << failRate << std::endl;

  return 0;
}