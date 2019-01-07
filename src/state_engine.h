// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Archetype engine manages states and rendering: inherited by e.g. MainMenuState
//


#ifndef STATE_ENGINE_H
#define STATE_ENGINE_H


#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "entity.h"


class State; // Forward declare; defined in src/g_states 


class StateEngine {
public:
	StateEngine(std::string title);
	~StateEngine();

	void change_state(State*);
	void push_state(State*);
	void pop_state();
	void clear_states();

	void handle_events();
	void update();
	void draw();

	bool is_running() { return bRunning; }
	void exit() { bRunning = false; }

	int get_stack_length() { return states.size(); }
		
	sf::RenderWindow window;					// Default view
	sf::View playFieldView;						// View for the player

private:
	std::stack<State*> states;

	bool bRunning;
	const int nWinWidth = 800;
	const int nWinHeight = 600;
	const int nFrameLimit = 60;
};


#endif
