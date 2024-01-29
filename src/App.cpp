#include "App.hpp"

using namespace Collision;

App::App(int *window_width, int *window_height, const char *window_name) {
  if (*window_width <= 0 || *window_height <= 0) {
    LOG::ERROR("one or more of the given window dimensions are too small (<= 0).", ERROR_FATAL);
    LOG::WARNING("Unless you have modified the source code, this should not be happening. This also might be due to memory corruption.");

    exit(EXIT_FAILURE);
  }

  if (strlen(window_name) == 0) {
    LOG::ERROR("the given window name is empty.", ERROR_FATAL);
    LOG::WARNING("Unless you have modified the source code, this should not be happening. This also might be due to memory corruption");

    exit(EXIT_FAILURE);
  }

  w_width = window_width;
  w_height = window_height;
  w_name = strdup(window_name);

  windowShouldClose = false;
  isRunning = true;

  //Check if the required asset files exist
  if (!FileExists("./assets/textures/asac.png") || !FileExists("./assets/textures/heisenberg.png") || 
      !FileExists("./assets/textures/background.png") || !FileExists("./assets/audio/asac_speak.mp3")
      || !FileExists("./assets/audio/heisenberg_speak.mp3") || !FileExists("./assets/audio/bb_theme.mp3") 
      || !FileExists("./assets/audio/bcs_theme.mp3")) {

    LOG::ERROR("one or more required assets files are missing. Please redownload the source code.", ERROR_FATAL);

    exit(EXIT_FAILURE);
  }

  LOG::INFO("Game succesfully initilized.");
}

void App::OnInit() {
  SetTraceLogLevel(LOG_ERROR);

  InitWindow(*w_width, *w_height, w_name);
  InitAudioDevice();

  delete w_width;
  delete w_height;
  delete w_name;

  SetTargetFPS(FPS);

  //Load the assets
  LoadAssets();

  // Init the Main Menu
  Menu::s_InitMainMenu();

  // Set the player starting pos. These values are just to put the player in the middle of the screen facing eachother
  asac.SetXPos(GetScreenWidth() / 2 - PLAYER_WIDTH - 5);
  asac.SetYPos(GetScreenHeight() / 2 - (PLAYER_HEIGHT / 2));

  heisenberg.SetXPos(GetScreenWidth() / 2 + 5);
  heisenberg.SetYPos(GetScreenHeight() / 2 - (PLAYER_HEIGHT / 2));
}

void App::LoadAssets() {
  // Load the background image
  Image img = LoadImage("./assets/textures/background.png");
  ImageResize(&img, GetScreenWidth(), GetScreenHeight());
  background_tex = LoadTextureFromImage(img);
  UnloadImage(img);

  // Load the asac player image
  img = LoadImage("./assets/textures/asac.png");
  ImageResize(&img, PLAYER_WIDTH, PLAYER_HEIGHT);
  asac_tex = LoadTextureFromImage(img);
  UnloadImage(img);

  // Load the heisenberg player image
  img = LoadImage("./assets/textures/heisenberg.png");
  ImageResize(&img, PLAYER_WIDTH, PLAYER_HEIGHT);
  heisenberg_tex = LoadTextureFromImage(img);
  UnloadImage(img);


  // Load the background themes
  bcs_theme = LoadSound("./assets/audio/bcs_theme.mp3");
  SetSoundVolume(bcs_theme, 0.2f);

  bb_theme = LoadSound("./assets/audio/bb_theme.mp3");
  SetSoundVolume(bb_theme, 0.2f);

  //Load the players speaking sounds
  asac_speak = LoadSound("./assets/audio/asac_speak.mp3");
  heisenberg_speak = LoadSound("./assets/audio/heisenberg_speak.mp3");
}

int App::OnExecute() {
  OnInit();

#ifndef NO_SOUND
  // start playing the background theme
  PlaySound(bb_theme);
#endif

  while (isRunning) {
    if (Menu::s_IsMenuActive()) {
      // stop the bcs theme from playing as the bb_theme should only be playing in the main menu.
      if (IsSoundPlaying(bcs_theme))
        StopSound(bcs_theme);

      // if the bb_theme stops playing, start it again in order to loop it forever
#ifndef NO_SOUND
      if (!IsSoundPlaying(bb_theme))
        PlaySound(bb_theme);
#endif

      // Draw the main menu
      BeginDrawing();

      DrawTexture(background_tex, 0, 0, WHITE);

      Menu::s_DrawMainMenu();

      EndDrawing();

      switch (Menu::s_PollMainMenuButtonEvents()) {
        case (EVENT_START_LOCAL_GAME):
          LOG::INFO("Starting local game...");
          Menu::s_SetMenuActiveState(false);

#ifndef NO_SOUND
          // start playing the background theme again when the game starts
          PlaySound(bcs_theme);
          PlaySound(bb_theme);
#endif

          // The player pos, health and bullets are reset.
          // If i dont do this, whenever the player goes back to the main menu and then later back to the game, the state of the game remains the same.
          asac.reset_health();
          heisenberg.reset_health();

          asac.SetWinState(false);
          heisenberg.SetWinState(false);

          asac_bullets.wipe();
          heisenberg_bullets.wipe();

          asac.SetXPos(GetScreenWidth() / 2 - PLAYER_WIDTH - 5);
          asac.SetYPos(GetScreenHeight() / 2 - (PLAYER_HEIGHT / 2));

          heisenberg.SetXPos(GetScreenWidth() / 2 + 5);
          heisenberg.SetYPos(GetScreenHeight() / 2 - (PLAYER_HEIGHT / 2));

          break;

        case (EVENT_START_MULTIPLAYER_GAME):
          LOG::INFO("multiplayer mode has not been implemented yet.");

          break;

        case (EVENT_EXIT):
          isRunning = false;

          break;

        case (EVENT_NOTHING):
          break;
      }
    } else {

      OnLoop();
      Draw();
      HandleInput();
    }
  }

  OnExit();

  return EXIT_SUCCESS;
}

void App::OnLoop() {
  // asac Player bullets
  for (auto &[x, y]: asac_bullets.GetBullets()) {
    x += BULLET_SPEED; // BULLET_SPEED defined in Bullet.hpp

    if (s_CheckCollision(std::make_pair(heisenberg.pos_x(), heisenberg.pos_y()),
          std::make_pair(x, y), PLAYER_WIDTH, PLAYER_HEIGHT, BULLET_WIDTH, BULLET_HEIGHT)) {

      // the heisenberg player has been hit. delete the bullet and add one health to asac, minus one health to heisenberg
      asac_bullets.del(std::make_pair(x, y));

      // MAX_HP defined in Player.hpp
      if (asac.GetHealth() < MAX_HP && (!asac.HasWon() && !heisenberg.HasWon())) {
        asac.add_health();
      }

      // Do not remove heisenberg's hp anymore if it is 0.
      if (heisenberg.GetHealth() > 0 && (!asac.HasWon() && !heisenberg.HasWon())) {
        heisenberg.rem_health();
      }

      // Check if asac has lost all of his HP
      if (heisenberg.GetHealth() == 0) {
        asac.SetWinState(true);
      }
#ifndef NO_SOUND
      // Play the asac speak
      PlaySound(asac_speak);
#endif

    } else if (x > GetScreenWidth()) {
      asac_bullets.del(std::make_pair(x, y));
    }
  }

  // heisenberg player bullets
  for (auto &[x, y]: heisenberg_bullets.GetBullets()) {
    x -= BULLET_SPEED;

    if (s_CheckCollision(std::make_pair(asac.pos_x(), asac.pos_y()), 
          std::make_pair(x, y), PLAYER_WIDTH, PLAYER_HEIGHT, BULLET_WIDTH, BULLET_HEIGHT)) {

      // the asac player has been hit. delete the bullet and add one health to heisenberg, minus one health to asac
      heisenberg_bullets.del(std::make_pair(x, y));

      // MAX_HP defined in Player.hpp
      if (heisenberg.GetHealth() < MAX_HP && (!heisenberg.HasWon() && !asac.HasWon())) {
        heisenberg.add_health();
      }

      // Do not remove asac's hp anymore if it is 0.
      if (asac.GetHealth() > 0 && (!heisenberg.HasWon() && !asac.HasWon())) {
        asac.rem_health();
      }

      // Check if heisenberg has lost all of his HP
      if (asac.GetHealth() == 0) {
        heisenberg.SetWinState(true);
      }

#ifndef NO_SOUND
      // play the heisenberg speak
      PlaySound(heisenberg_speak);
#endif

    } else if (x < 0) {
      heisenberg_bullets.del(std::make_pair(x, y));
    }
  }

  // winner checking
  if (asac.HasWon() || heisenberg.HasWon()) {
    // Wait to space to be pressed to start the game again.
    if (IsKeyPressed(KEY_SPACE)) {
      asac.reset_health();
      heisenberg.reset_health();

      asac.SetWinState(false);
      heisenberg.SetWinState(false);

      asac_bullets.wipe();
      heisenberg_bullets.wipe();

      asac.SetXPos(GetScreenWidth() / 2 - PLAYER_WIDTH - 5);
      asac.SetYPos(GetScreenHeight() / 2 - (PLAYER_HEIGHT / 2));

      heisenberg.SetXPos(GetScreenWidth() / 2 + 5);
      heisenberg.SetYPos(GetScreenHeight() / 2 - (PLAYER_HEIGHT / 2));
#ifndef NO_SOUND
      PlaySound(bcs_theme);
      PlaySound(bb_theme);
#endif
    }
  }

  // check if the player posistions are negative. this was causing issues with the collision function.
  asac.SetXPos((asac.pos_x() < 0 ) ? 0 : asac.pos_x());
  asac.SetYPos((asac.pos_y() < 0) ? 0 : asac.pos_y());

  heisenberg.SetXPos((heisenberg.pos_x() < 0) ? 0 : heisenberg.pos_x());
  heisenberg.SetYPos((heisenberg.pos_y() < 0) ? 0 : heisenberg.pos_y());

  // if the background music stops playing, start it again.
#ifndef NO_SOUND
  if (!IsSoundPlaying(bb_theme))
    PlaySound(bb_theme);
  if (!IsSoundPlaying(bcs_theme))
    PlaySound(bcs_theme);
#endif
}

void App::HandleInput() {
  if (WindowShouldClose()) {
    windowShouldClose = true;
  }

  // Input for asac player. PLAYER_SPEED defined in Player.hpp
  if (IsKeyDown(KEY_W) && asac.pos_y() > 0)
    asac.SetYPos(asac.pos_y() - PLAYER_SPEED);
  if (IsKeyDown(KEY_S) && asac.pos_y() < GetScreenHeight() - PLAYER_HEIGHT)
    asac.SetYPos(asac.pos_y() + PLAYER_SPEED);
  if (IsKeyDown(KEY_A) && asac.pos_x() > 0)
    asac.SetXPos(asac.pos_x() - PLAYER_SPEED);
  if (IsKeyDown(KEY_D) && asac.pos_x() < GetScreenWidth() / 2 - PLAYER_WIDTH - 5) // the asac player should not go over the middle screen line. the (-5) is because of the line width
    asac.SetXPos(asac.pos_x() + PLAYER_SPEED);

  if (IsKeyPressed(KEY_F))
    asac_bullets.append(std::make_pair(asac.pos_x() + PLAYER_WIDTH, asac.pos_y() + PLAYER_HEIGHT / 2));

  // Input for heisenberg player.
  if (IsKeyDown(KEY_UP) && heisenberg.pos_y() > 0)
    heisenberg.SetYPos(heisenberg.pos_y() - PLAYER_SPEED);
  if (IsKeyDown(KEY_DOWN) && heisenberg.pos_y() < GetScreenHeight() - PLAYER_HEIGHT)
    heisenberg.SetYPos(heisenberg.pos_y() + PLAYER_SPEED);
  if (IsKeyDown(KEY_LEFT) && heisenberg.pos_x() > GetScreenWidth() / 2 + 5) // the heisenberg player should not go over the middle screen line. the (+ 5) is because of the line width
    heisenberg.SetXPos(heisenberg.pos_x() - PLAYER_SPEED);
  if (IsKeyDown(KEY_RIGHT) && heisenberg.pos_x() < GetScreenWidth() - PLAYER_WIDTH) 
    heisenberg.SetXPos(heisenberg.pos_x() + PLAYER_SPEED);

  if (IsKeyPressed(KEY_SLASH))
    heisenberg_bullets.append(std::make_pair(heisenberg.pos_x(), heisenberg.pos_y() + PLAYER_HEIGHT / 2));
}

void App::Draw() {
    BeginDrawing();
  ClearBackground(WHITE);

  //Draw the background
  DrawTexture(background_tex, 0, 0, WHITE); 

  // Draw the players
  DrawTexture(asac_tex, asac.pos_x(), asac.pos_y(), WHITE);
  DrawTexture(heisenberg_tex, heisenberg.pos_x(), heisenberg.pos_y(), WHITE);

  DrawLineEx(Vector2{(float)GetScreenWidth() / 2, 0}, Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight()}, 5.0, BLACK);

  // Draw the player bullets.
  asac_bullets.DrawBullets();
  heisenberg_bullets.DrawBullets();

  // Draw the players HP count.
  DrawText(TextFormat("HP: %d", asac.GetHealth()), 40, 40, 40, BLACK);
  DrawText(TextFormat("HP: %d", heisenberg.GetHealth()), GetScreenWidth() - 200, 40, 40, BLACK);

  if (asac.HasWon()) {
    DrawText("ASAC Schrader has won! Press Space to play again.", GetScreenWidth() / 4 - 200, GetScreenHeight() / 2 - 50 / 2, 50, BLACK);
  } else if (heisenberg.HasWon()) {
    DrawText("Heisenberg has won! Press Space to play again.", GetScreenWidth() / 4 - 200, GetScreenHeight() / 2 - 50 / 2, 50, BLACK);
  }

  // Check if the window should close and promt the user if they are sure.
  // This must be done in the drawing function
  if (windowShouldClose) {
    switch (Menu::s_ShowQuitPopUpMenu()) {
      case (EVENT_CONTINUE_PLAYING): // if the user presses the x button and closes the popup.
        windowShouldClose = false;
        break;

      case (EVENT_EXIT): // if the user chooses to quit the game
        isRunning = false;
        break;

      case (EVENT_BACK_TO_MAIN_MENU): // if the user deceides to go back to the main menu.
        windowShouldClose = false;
        Menu::s_SetMenuActiveState(true);

        // Start playing the menu theme again.
#ifndef NO_SOUND
        PlaySound(bb_theme);
#endif

        break;

      case (EVENT_NOTHING): // if the user does nothing
        break;
    }
  }

  EndDrawing();
}

void App::OnExit() const {
  UnloadTexture(background_tex);
  UnloadTexture(asac_tex);
  UnloadTexture(heisenberg_tex);

  UnloadSound(asac_speak);
  UnloadSound(heisenberg_speak);

  UnloadSound(bb_theme);
  UnloadSound(bcs_theme);

  CloseWindow();
}
