#pragma once

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <iostream>

namespace Collision {
  static inline bool CheckCollision(std::pair<short int, short int> rec1, std::pair<short int, short int> rec2, short int rec1_width, short int rec1_height, short int rec2_width, short int rec2_height) {
    bool collide = false;

    //std::cout << "rec1_width: " << rec1_width << ". rec1_height: " << rec1_height << '\n';
    //std::cout << "rec1 pos: (x=" << rec1.first << "; y=" << rec1.second << ")\n";
    //std::cout << "rec2 pos: (x=" << rec2.first << "; y=" << rec2.second << ")\n";

    if ((rec1.first < (rec2.first + rec2_width) && (rec1.first + rec1_width) > rec2.first) &&
           (rec1.second < (rec2.second + rec2_height) && (rec1.second + rec1_height) > rec2.second)) {
           collide = true;
    }

    return collide;
  }
};

#endif

