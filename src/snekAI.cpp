#include "game.h"
#include <iostream>

/**
 * The below is a path for a hamiltonian path on a 8x8 grid.
 * >>>>>>>v
 * ^v<<<<<<
 * ^>>>>>>v
 * ^v<<<<<<
 * ^>>>>>>v
 * ^v<<<<<<
 * ^>>>>>>v
 * ^<<<<<<<
 */

// for each index, it is the index of the next cell in the path
constexpr uint8_t hamiltonian[64] = {
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    15,
    0,
    17,
    9,
    10,
    11,
    12,
    13,
    14,
    8,
    18,
    19,
    20,
    21,
    22,
    23,
    31,
    16,
    33,
    25,
    26,
    27,
    28,
    29,
    30,
    24,
    34,
    35,
    36,
    37,
    38,
    39,
    47,
    32,
    49,
    41,
    42,
    43,
    44,
    45,
    46,
    40,
    50,
    51,
    52,
    53,
    54,
    55,
    63,
    48,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
};

uint8_t snekAI(Snek snek, uint8_t food)
{

  static bool cached = false;
  static int8_t cycleStep[64];
  static int8_t neighbors[64][4];

  uint8_t head = snek.x.back();

  std::bitset<64> snek_mask = snek.x_mask;

  // remove the tail from the mask
  snek_mask[snek.x.front()] = 0;

  if (!cached)
  {
    int k = 0;
    int step = 0;
    while (hamiltonian[k] != 0)
    {
      cycleStep[k] = step;
      step++;
      k = hamiltonian[k];
    }
    cycleStep[k] = step;

    for (int i = 0; i < 64; i++)
    {
      // default to -1
      neighbors[i][0] = -1;
      neighbors[i][1] = -1;
      neighbors[i][2] = -1;
      neighbors[i][3] = -1;

      if (i % 8 != 0)
        neighbors[i][0] = i - 1;
      if (i % 8 != 7)
        neighbors[i][1] = i + 1;
      if (i / 8 != 0)
        neighbors[i][2] = i - 8;
      if (i / 8 != 7)
        neighbors[i][3] = i + 8;
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
  if (snek.length > 32)
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
      foodStep = 63;

    // we must never backtrack
    bool isValid = step > headStep;

    // tail protection - don't go after the tail
    bool tailProtection = (tailStep < headStep && step < tailStep) || (tailStep > headStep && step > tailStep);

    bool foodProtection = step < foodStep;

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