// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Archetype engine manages states and rendering: inherited by e.g. MainMenuState
//
// TODO: 
//  (jhelland) Virtualize to allow multiple engines on one window?
//  (jhelland) Allow engine to handle multiple states at once?


#ifndef STATE_ENGINE_H
#define STATE_ENGINE_H


#include <stack>
#include <unordered_map>
#include <uuid.h>  // stduuid library: https://github.com/mariusbancila/stduuid

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
	int get_entities_length() { return entities.size(); }
	void add_entity(Entity* entity);
	Entity* get_entity_by_id(uuids::uuid& id);
	void clear_entities();
	
	sf::RenderWindow window;					// Default view
	sf::View playFieldView;						// View for the player
	std::vector<uuids::uuid> entitiesDist0;		// An example of a list of entities w/in update range

private:
	std::stack<State*> states;
	std::unordered_map<uuids::uuid, Entity*> entities;

	bool bRunning;
	const int nWinWidth = 800;
	const int nWinHeight = 600;
	const int nFrameLimit = 60;
};


#endif
