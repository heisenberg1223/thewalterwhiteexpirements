#pragma once

#ifndef BULLET_HPP
#define BULLET_HPP

#include <raylib.h>
#include <vector>
#include <utility>
#include <algorithm>

#include "Config/Config.hpp"

class Bullet {
  public:
    // return the bullet vector
    auto& GetBullets() { return bullets; };

    // draw the bullets
    void DrawBullets() const {
      for (auto &[x, y] : bullets) {
        DrawRectangle(x, y, BULLET_WIDTH, BULLET_HEIGHT, YELLOW);
      }
    }

    // append a bullet to the end of the bullet vector
    void append(std::pair<short int, short int> pair) {
      bullets.push_back(pair);
    }

    // delete a element from the bullet vector
    void del(std::pair<short int, short int> pair) {
      auto it = std::find(bullets.begin(), bullets.end(), pair);
      if (it != bullets.end()) {
        bullets.erase(it);
      }
    }

    // delete all elements from the bullets vector
    void wipe() { bullets.clear(); };

  private:
    std::vector<std::pair<short int, short int>> bullets;
};

#endif
