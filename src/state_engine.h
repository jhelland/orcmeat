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

#include "core/entity.h"


class State; // Forward declare; defined in src/g_states 


class StateEngine {
private:
	sf::Clock deltaClock;
	std::stack<State*> states;

	bool bRunning;
	const int nWinWidth = 800;
	const int nWinHeight = 600;
	const int nFrameLimit = 60;

public:
	sf::RenderWindow window;
	sf::View playerView;

public:
	StateEngine(std::string title);
	~StateEngine(); 

	// State management
	void change_state(State*);
	void push_state(State*);
	void pop_state();
	void clear_states();

	void handle_events();
	void update();
	void draw();

	inline bool is_running() { return bRunning; }
	void exit();

	// Utility functions
	inline int get_stack_length() const { return states.size(); }
	inline const sf::Vector2i get_window_dimensions() const { return sf::Vector2i(nWinWidth, nWinHeight); }
	inline const sf::Rect<float> get_view_box(const sf::View& view) const {
		auto center = view.getCenter();
		auto size = view.getSize();
		return sf::Rect<float>(center - size / 2.f, size);
	}

	float get_delta_time() { return deltaClock.restart().asSeconds(); }
};


#endif
