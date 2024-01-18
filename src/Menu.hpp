#pragma once

#ifndef MENU_HPP
#define MENU_HPP


#include "Log/LogCallback.hpp"
#include "Player.hpp"

#include <raylib.h>
#include <iostream>

#define BUTTON_SPACING 60

// all the possible button events
enum ButtonEvents {
  EVENT_NOTHING,
  EVENT_EXIT,
  EVENT_START_LOCAL_GAME,
  EVENT_START_MULTIPLAYER_GAME, // To be implemented later
  
  EVENT_BACK_TO_MAIN_MENU,
  EVENT_CONTINUE_PLAYING
};

class Menu {
  public:
    // Load the main menu and init raygui 
    static void s_InitMainMenu();

    // reset all of the button pressed bools
    static void s_ResetMainMenu();

    // Draw the main menu
    static void s_DrawMainMenu();

    // Return a ButtonEvent depending on what button is currently active.
    static ButtonEvents s_PollMainMenuButtonEvents();

    // return the state of the menu.
    static bool s_IsMenuActive() { return isMenuActive; };
    static void s_SetMenuActiveState(bool state) { isMenuActive = state; };

    // ### Popup menu functions ###

    // Show a popup function that shows each players stats after the game. The user will be asked if they want to return to the main menu or play again.
    // The playerName argument tells the function which player has won.
    static void s_ShowPostGamePopUpMenu(PlayerNames playerName);

    // draw the quit message and return the option choosed.
    static ButtonEvents s_ShowQuitPopUpMenu();

  private:
    static bool isLocalPressed, isMultiplayerPressed, isExitPressed, isMenuActive;

    static Font s_textFont;

    // The width and height of the buttons.
    static const float s_buttonWidth, s_buttonHeight;

    // This stores the x and y pos of the top button. the buttons after that are incremented in the y axis with BUTTON_SPACING
    static float s_buttonXPos, s_buttonYPos;
};

#endif
