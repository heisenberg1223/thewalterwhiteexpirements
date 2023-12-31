#pragma once

#ifndef BULLET_HPP
#define BULLET_HPP

#include <raylib.h>
#include <vector>
#include <utility>
#include <algorithm>

#define BULLET_WIDTH 50
#define BULLET_HEIGHT 5
#define BULLET_SPEED 12

class Bullet {
  public:
    auto& GetBullets() { return bullets; };

    void DrawBullets() const {
      for (auto &[x, y] : bullets) {
        DrawRectangle(x, y, BULLET_WIDTH, BULLET_HEIGHT, YELLOW);
      }
    }

    void append(std::pair<short int, short int> pair) {
      bullets.push_back(pair);
    }

    void del(std::pair<short int, short int> pair) {
      auto it = std::find(bullets.begin(), bullets.end(), pair);
      if (it != bullets.end()) {
        bullets.erase(it);
      }
    }

    void wipe() {
      bullets.clear();
    }
  private:
    std::vector<std::pair<short int, short int>> bullets;
};

#endif
