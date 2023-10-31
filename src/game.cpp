#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

SnekGame::SnekGame()
{
  // initialize snek
  snek.x.push_back(40);
  snek.x_mask[40] = 1;

  snek.length = 1;
  snek.direction = RIGHT;

  // initialize food
  spawnFood();

  // initialize game state
  game_over = false;
  score = 0;
  ticks = 0;

  // initialize random number generator
  srand(time(NULL));
}

SnekGame::~SnekGame(){
    // nothing to do here
};

void SnekGame::spawnFood()
{
  // id the snek is full, don't spawn food
  if (snek.length == 400)
  {
    return;
  }

  while (true)
  {
    // attempt to spawn food at random location
    food = rand() % 400;

    // check if food is in snek
    if (snek.x_mask[food] == 0)
    {
      break;
    }
  }
}

void SnekGame::growSnek()
{
  // grow snek
  snek.length++;
  snek.x.push_back(food);

  // for some reason
  // the mask gets out of sync
  // regenerate
  snek.x_mask.reset();

  for (int i = 0; i < snek.length; i++)
  {
    snek.x_mask[snek.x[i]] = 1;
  }
}

void SnekGame::movePos(uint16_t new_head)
{

  // check if snek is dead
  if (game_over)
  {
    return;
  }

  // is it colliding with itself?
  std::bitset<400> snek_mask = snek.x_mask;

  // remove the tail from the mask
  snek_mask[snek.x.front()] = 0;

  if (snek_mask[new_head] == 1)
  {
    game_over = true;
    return;
  }

  // check if snek is eating food
  if (new_head == food)
  {
    growSnek();
    spawnFood();
  }
  else
  {
    // update snek mask
    snek.x_mask[new_head] = 1;
    snek.x.push_back(new_head);

    // remove tail
    snek.x_mask[snek.x.front()] = 0;
    snek.x.pop_front();
  }

  // if we hit 400, we win
  if (snek.length == 400)
  {
    game_over = true;
  }
}

void SnekGame::moveSnek(Direction d)
{

  // get the current head
  uint16_t head = snek.x.back();

  // figure out where the new head is
  uint16_t new_head = head;

  if (d == UP)
  {
    if (head < 20)
    {
      game_over = true;
      return;
    }

    new_head = head - 20;
  }
  else if (d == DOWN)
  {
    if (head > 179)
    {
      game_over = true;
      return;
    }

    new_head = head + 20;
  }
  else if (d == LEFT)
  {
    if (head % 20 == 0)
    {
      game_over = true;
      return;
    }

    new_head = head - 1;
  }
  else if (d == RIGHT)
  {
    if ((head + 1) % 20 == 0)
    {
      game_over = true;
      return;
    }

    new_head = head + 1;
  }

  movePos(new_head);
}

void SnekGame::nextTick()
{
  // get the next cell from the AI
  uint16_t d = snekAI(snek, food);

  // move the snek
  movePos(d);

  // update score
  score = snek.length;

  // update ticks
  ticks++;
}

void SnekGame::reset()
{
  // reset snek
  snek.x.clear();
  snek.x_mask.reset();
  snek.x.push_back(40);
  snek.x_mask[40] = 1;
  snek.length = 1;
  snek.direction = RIGHT;

  // reset food
  spawnFood();

  // reset game state
  game_over = false;
  score = 0;
  ticks = 0;
}