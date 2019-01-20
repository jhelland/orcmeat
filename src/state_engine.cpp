// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Archetype state machine manager.


#ifdef _DEBUG
#include <iostream>
#endif

#include "state_engine.h"
#include "g_states.h"


StateEngine::StateEngine(std::string title) {
	// initalize window 
	window.create(sf::VideoMode(nWinWidth, nWinHeight), title, sf::Style::Titlebar | sf::Style::Close); // leaving out Resize flag turns off resizing
	window.setFramerateLimit(nFrameLimit);
	window.setVerticalSyncEnabled(true);
	window.setJoystickThreshold(20.f);
	bRunning = true;

	// Player view
	playerView.reset(sf::FloatRect(
		0.f, 0.f, static_cast<float>(nWinWidth), static_cast<float>(nWinHeight)
	));

	window.setView(playerView);
}


StateEngine::~StateEngine() {
	clear_states();
}


void StateEngine::change_state(State* state) {
	if (!states.empty()) {
		states.top()->clean();
		states.pop();
	}
	states.push(state);
	states.top()->initialize();
}


void StateEngine::push_state(State* state) {
	states.push(state);
	states.top()->initialize();
}


void StateEngine::pop_state() {
	if (!states.empty()) {
		states.top()->clean();
		states.pop();
	}
	if (!states.empty()) {
		states.top()->resume();
	} else {
		bRunning = false;
	}
}


void StateEngine::clear_states() {
	while (!states.empty()) {
		states.top()->clean();
		states.pop();
	}
}


// handle window closing, switching states, etc.
void StateEngine::handle_events() {
	if (!states.empty()) {
		states.top()->handle_events(this);
	}
}


void StateEngine::update() {
	if (!states.empty()) {
		states.top()->update(this);
	}
}


void StateEngine::draw() {
	if (!states.empty()) {
		window.clear();
		states.top()->draw(this);
		window.display();
	}
}


void StateEngine::exit() {
	bRunning = false;
}
