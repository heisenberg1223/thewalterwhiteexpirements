#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <raylib.h>

#include "Config/Config.hpp"

class Player {
  public:
    // constructor
    Player();

    // return the x or y pos of the player.
    short int pos_x() { return pos.x; };
    short int pos_y() { return pos.y; };

    // Set the x or y pos of the player.
    void SetXPos(const int x) { pos.x = x; };
    void SetYPos(const int y) { pos.y = y; };

    // increase the players health by one
    inline void add_health() { health++; };

    // decrement the players health by one.
    inline void rem_health() { health--; };

    // reset the players health
    inline void reset_health() { health = STARTING_HP; };

    // return the players health
    unsigned short int GetHealth() { return health; };

    // return the player won state
    bool HasWon() { return hasWon; };

    // set the player win state
    inline void SetWinState(bool state) { hasWon = state; };

  private:
    struct pos {
      short int x, y;
    } pos;

    unsigned short int health;

    bool hasWon;
};

#endif
