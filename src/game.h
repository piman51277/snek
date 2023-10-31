#pragma once
#include <deque>
#include <cinttypes>
#include <bitset>

enum Direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

// board is 20x20
struct Snek
{
  std::deque<uint16_t> x;  // index of each cell in snek (in order)
  std::bitset<400> x_mask; // each bit is 1 if snek occupies cell, 0 if not
  uint16_t length;
  Direction direction;
};

uint16_t snekAI(Snek snek, uint16_t food);

class SnekGame
{
public:
  Snek snek;
  uint16_t food;
  bool game_over;
  int score;
  int ticks;

  SnekGame();
  ~SnekGame();
  void nextTick();
  void reset();

private:
  void spawnFood();
  void growSnek();
  void moveSnek(Direction d);
  void movePos(uint16_t c);
};
