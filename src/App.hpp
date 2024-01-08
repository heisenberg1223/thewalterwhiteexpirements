#pragma once 

#ifndef APP_HPP
#define APP_HPP

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <cstring>

#include "Log/LogCallback.hpp"
#include "Player.hpp"
#include "Bullet.hpp"
#include "Collision.hpp"

#define FPS 60

class App {
  public:
    App(int *window_width, int *window_height, const char *window_name);

    void OnInit();
    void LoadAssets();
    int OnExecute();
    void OnExit();

    void HandleInput();
    void OnLoop();
    void Draw();

    void CallWinner(Player &player);

  private:
    int *w_width = new int;
    int *w_height = new int;

    char *w_name = new char;

    bool isRunning;

    Texture2D background_tex, asac_tex, heisenberg_tex;

    Sound bb_theme, bcs_theme, asac_speak, heisenberg_speak;

    Player asac;
    Bullet asac_bullets;

    Player heisenberg;
    Bullet heisenberg_bullets;
};

#endif
