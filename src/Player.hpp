#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <raylib.h>

#define PLAYER_SPEED 9

#define PLAYER_WIDTH 100
#define PLAYER_HEIGHT 125

#define STARTING_HP 10
#define MAX_HP 15

class Player {
  public:
    Player();

    short int pos_x() { return pos.x; };
    short int pos_y() { return pos.y; };

    void SetXPos(const int x) { pos.x = x; };
    void SetYPos(const int y) { pos.y = y; };

    inline void add_health() {
      health++;
    }

    inline void rem_health() {
      health--;
    }

    inline void reset_health() {
      health = STARTING_HP;
    }

    inline unsigned short int GetHealth() {
      return health;
    }

    inline bool HasWon() {
      return hasWon;
    }

    void SetWinState(bool state) {
      hasWon = state;
    }

  protected:
    struct pos {
      short int x, y;
    } pos;

    unsigned short int health;

    bool hasWon;
};

#endif
