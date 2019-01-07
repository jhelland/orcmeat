// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Archetype state machine manager.


#include <iostream>

#include "state_engine.h"
#include "g_states.h"


// initializer
StateEngine::StateEngine(std::string title) {
	// initalize window 
	window.create(sf::VideoMode(nWinWidth, nWinHeight), title, sf::Style::Titlebar | sf::Style::Close); // leaving out Resize flag turns off resizing
	window.setFramerateLimit(nFrameLimit);
	bRunning = true;

	// intialize player view
	// default view will be used for HUD
	playFieldView.reset(sf::FloatRect(
		0.f, 0.f, static_cast<float>(nWinWidth), static_cast<float>(nWinHeight)
	));
}


// destructor
StateEngine::~StateEngine() {
	while (!states.empty()) {
		states.top()->clean();
		states.pop();
	}
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
	states.top()->handle_events(this);
}


void StateEngine::update() {
	states.top()->update(this);
}


void StateEngine::draw() {
	window.clear();
	states.top()->draw(this);
	window.display();
}
