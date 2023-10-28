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

// board is 8x8
struct Snek
{
  std::deque<uint8_t> x;  // index of each cell in snek (in order)
  std::bitset<64> x_mask; // each bit is 1 if snek occupies cell, 0 if not
  uint8_t length;
  Direction direction;
};

uint8_t snekAI(Snek snek, uint8_t food);

class SnekGame
{
public:
  Snek snek;
  uint8_t food;
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
  void movePos(uint8_t c);
};
