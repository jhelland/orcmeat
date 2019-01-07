// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  State manager for the main game.
//


#ifndef GAME_H
#define GAME_H


#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "main_menu_state.h"
#include "pause_menu_state.h"
#include "entity_circle.h"


class GameState : public State {
public:
	void initialize();
	void clean();

	void pause();
	void resume();

	void update(StateEngine*);
	void draw(StateEngine*);
	void handle_events(StateEngine*);
	static GameState* instance() { return &g_state; } // allow for convenient switching between states

protected:
	GameState() { }

private:
	static GameState g_state; // Static to prevent creating new copies with every instance of the state
	sf::Texture background_img;
	sf::Sprite background;
};


#endif 
