#include "Player.hpp"

Player::Player() {
  pos.x = 0;
  pos.y = 0;

  health = STARTING_HP;
  won = false;
}
