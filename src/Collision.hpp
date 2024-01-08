#pragma once

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <utility>

namespace Collision {
  static inline bool s_CheckCollision(std::pair<short int, short int> rec1, std::pair<short int, short int> rec2, short int rec1_width, short int rec1_height, short int rec2_width, short int rec2_height) {
    bool collide = false;

    if ((rec1.first < (rec2.first + rec2_width) && (rec1.first + rec1_width) > rec2.first) &&
           (rec1.second < (rec2.second + rec2_height) && (rec1.second + rec1_height) > rec2.second)) {
           collide = true;
    }

    return collide;
  }
};

#endif

