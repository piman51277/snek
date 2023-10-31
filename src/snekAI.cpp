#include "game.h"
#include <iostream>

uint16_t snekAI(Snek snek, uint16_t food)
{

  static bool cached = false;
  static uint16_t hamiltonian[400];
  static uint16_t cycleStep[400];
  static int16_t neighbors[400][4];

  uint16_t head = snek.x.back();

  std::bitset<400> snek_mask = snek.x_mask;

  // remove the tail from the mask
  snek_mask[snek.x.front()] = 0;

  if (!cached)
  {
    // build the hamiltonian cycle

    // first line is
    //>>>> ... >>>v
    for (int i = 0; i <= 18; i++)
    {
      hamiltonian[i] = i + 1;
    }
    hamiltonian[19] = 19 + 20;

    // next lines are alternating
    //^v<<<<<<<<<<<<<<
    //^>>>>>>>>>>>>>>v

    for (int y = 1; y <= 18; y++)
    {
      if (y % 2 == 1)
      {
        hamiltonian[y * 20] = (y - 1) * 20;
        hamiltonian[y * 20 + 1] = (y + 1) * 20 + 1;

        for (int x = 2; x <= 19; x++)
        {
          hamiltonian[y * 20 + x] = y * 20 + x - 1;
        }
      }
      else
      {
        hamiltonian[y * 20] = (y - 1) * 20;

        for (int x = 1; x <= 19; x++)
        {
          hamiltonian[y * 20 + x] = y * 20 + x + 1;
        }

        hamiltonian[y * 20 + 19] = (y + 1) * 20 + 19;
      }
    }

    // last line is
    //^<<<<<<<<<<<
    hamiltonian[19 * 20] = 18 * 20;
    for (int i = 1; i <= 19; i++)
    {
      hamiltonian[19 * 20 + i] = 19 * 20 + i - 1;
    }

    int k = 0;
    int step = 0;
    while (hamiltonian[k] != 0)
    {
      cycleStep[k] = step;
      step++;
      k = hamiltonian[k];
    }
    cycleStep[k] = step;

    for (int i = 0; i < 400; i++)
    {
      // default to -1
      neighbors[i][0] = -1;
      neighbors[i][1] = -1;
      neighbors[i][2] = -1;
      neighbors[i][3] = -1;

      if (i % 20 != 0)
        neighbors[i][0] = i - 1;
      if (i % 20 != 19)
        neighbors[i][1] = i + 1;
      if (i / 20 != 0)
        neighbors[i][2] = i - 20;
      if (i / 20 != 19)
        neighbors[i][3] = i + 20;
    }
  }

  // find the cycle step of the food
  int foodStep = cycleStep[food];

  // step of the head
  int headStep = cycleStep[head];

  // step of the tail
  int tailStep = cycleStep[snek.x.front()];

  // look at neighbors of head and get thier cycle steps
  int neighborSteps[4] = {-1, -1, -1, -1};
  for (int i = 0; i < 4; i++)
  {
    int neighbor = neighbors[head][i];
    if (neighbor != -1 && snek_mask[neighbor] == 0)
    {
      neighborSteps[i] = cycleStep[neighbor];
    }
  }

  // is there food in the remaining cycle?
  bool doOp = foodStep > headStep;

  // if there is no food in the remaining cycle
  // or we already filled most of the board
  // just do regular hamiltonian path
  if (snek.length > 200)
    return hamiltonian[head];

  // find the neighbor with the largest step
  // that is valid
  int maxStep = 0;
  int maxStepIndex = -1;
  for (int i = 0; i < 4; i++)
  {
    int step = neighborSteps[i];

    // if not, set a ghost food at the cycle reset
    if (!doOp)
      foodStep = 399;

    // we must never backtrack
    bool isValid = step > headStep;

    // tail protection - don't go after the tail
    bool tailProtection = (tailStep < headStep && step < tailStep) || (tailStep > headStep && step > tailStep);

    bool foodProtection = step <= foodStep;

    if (step > maxStep && step != -1 && isValid && !tailProtection && foodProtection)
    {
      maxStep = neighborSteps[i];
      maxStepIndex = neighbors[head][i];
    }
  }

  // if there is no valid neighbor,
  // fall back to hamiltonian path
  if (maxStepIndex == -1)
    return hamiltonian[head];

  // use the picked neighbor
  return maxStepIndex;
}