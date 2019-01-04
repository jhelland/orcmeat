// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main driver: initiates state manager for game


#include "state_engine.h"
#include "main_menu_state.h"

#include <iostream>


int main () 
{
  StateEngine gameStateEngine("Orc Meat");

	gameStateEngine.change_state(MainMenu::instance());

	while (gameStateEngine.is_running()) 
  {
		gameStateEngine.handle_events();
		gameStateEngine.update();
		gameStateEngine.draw();
	}

  std::cout << "Game exited\n";

  gameStateEngine.window.close();
}


