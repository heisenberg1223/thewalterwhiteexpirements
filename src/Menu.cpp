#include "Menu.hpp"
#include <raylib.h>

/* I know this seems weird, but for some reason the preprocessor directives in the raygui file do not work.
 * I tried to add these lines in the Menu.hpp file, but it would not compile due to redefined Functions.
 */
#ifndef RAYGUI_FILE
#define RAYGUI_FILE

#define RAYGUI_IMPLEMENTATION
#include "../lib/raygui/src/raygui.h"

#endif

bool Menu::isLocalPressed = false;
bool Menu::isMultiplayerPressed = false;
bool Menu::isExitPressed = false;
bool Menu::isMenuActive = true;

Font Menu::s_textFont;

const float Menu::s_buttonWidth = 380, Menu::s_buttonHeight = 48;

float Menu::s_buttonXPos, Menu::s_buttonYPos;

void Menu::s_InitMainMenu() {
  if (FileExists("./assets/font/LiberationMono-Bold.ttf")) {
    // Load the font that will be used in the main menu
    s_textFont = LoadFontEx("./assets/font/LiberationMono-Bold.ttf", 15, 0, 0);
    GuiSetFont(s_textFont);
  } else { // if the required font file could not be found...
    LOG::ERROR("the required font could not be found. please redownload the source code.", ERROR_FATAL);
    exit(EXIT_FAILURE);
  }

  GuiSetStyle(DEFAULT, TEXT_SIZE, 15); // Set the GUI text size

  s_buttonXPos = GetScreenWidth() / 2 - s_buttonWidth / 2;
  s_buttonYPos = GetScreenHeight() / 2 - s_buttonHeight / 2;
}

void Menu::s_ResetMainMenu() {
  isLocalPressed = false;
  isMultiplayerPressed = false;
  isExitPressed = false;
}

void Menu::s_DrawMainMenu() {
  ClearBackground(RAYWHITE);

  // Draw a rectangle around the menu text.
  DrawRectangleRec(Rectangle{GetScreenWidth() / 2 - 600 / 2, 200, 600, 200}, BLACK);

  // Draw the game name
  DrawTextEx(s_textFont, "The Walter White Expirements",
      Vector2{GetScreenWidth() / 2  - MeasureTextEx(s_textFont, "The Walter White Expirements", s_textFont.baseSize, 10).x / 2, 300}, 15, 10, WHITE);
  
  isLocalPressed = GuiButton(Rectangle{s_buttonXPos, s_buttonYPos, s_buttonWidth, s_buttonHeight}, "Local Co-op");
  isMultiplayerPressed = GuiButton(Rectangle{s_buttonXPos, s_buttonYPos + BUTTON_SPACING, s_buttonWidth, s_buttonHeight}, "Co-op over Wi-Fi");
  isExitPressed = GuiButton(Rectangle{s_buttonXPos, s_buttonYPos + BUTTON_SPACING * 2, s_buttonWidth, s_buttonHeight}, "Exit");
}

ButtonEvents Menu::s_PollMainMenuButtonEvents() {
  if (isLocalPressed) return EVENT_START_LOCAL_GAME;
  else if (isMultiplayerPressed) return EVENT_START_MULTIPLAYER_GAME;
  else if (isExitPressed) return EVENT_EXIT;
  else return EVENT_NOTHING;
}

ButtonEvents Menu::s_ShowQuitPopUpMenu() {
  int status = GuiMessageBox(Rectangle{GetScreenWidth() / 2 - 520 / 2, GetScreenHeight() / 2 - 120 / 2, 520, 120},
      "#159#Back to main menu?", "What would you like to do?", "Quit the game;Continue Playing;Back to Main Menu");

  switch (status) {
    case (0): // if the user presses the x button to close the popup window.
      return EVENT_CONTINUE_PLAYING;

      break;

    case (1): // if the user chooses to exit the game
      return EVENT_EXIT;

      break;

    case (2): // if the user chooses to continue playing
      return EVENT_CONTINUE_PLAYING;

      break;

    case (3): // if the user chooses to go back to the main menu
      return EVENT_BACK_TO_MAIN_MENU;

      break;

    default: // if the user does nothing
      return EVENT_NOTHING;
  }
}

