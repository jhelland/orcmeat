// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main driver: initiates state manager for game


// Debug

#include<iostream>

#include "state_engine.h"
#include "main_menu_state.h"
#include "core/core.h"

#include <vld.h>


int main() {
	core::memory::alloc_program_memory(104857600);  // 100 mb

	static StateEngine gameStateEngine("Orc Meat");

	gameStateEngine.change_state(MainMenu::instance());

	while (gameStateEngine.is_running()) {
		gameStateEngine.handle_events();
		gameStateEngine.update();
		gameStateEngine.draw();
	}

	gameStateEngine.window.close();

	std::cout << "Game exited\n";

	core::memory::free_program_memory();
}


