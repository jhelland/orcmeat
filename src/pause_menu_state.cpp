// Created by jhelland (12/21/18)
// 
// DESCRIPTION:
//  State manager for the pause menu.
//


#include <iostream>

#include "globals.h"
#include "pause_menu_state.h"


PauseMenuState PauseMenuState::state;


void PauseMenuState::initialize() {
	p_circ.setRadius(30);
	p_circ.setFillColor(sf::Color::Green);
}


void PauseMenuState::clean() {

}


void PauseMenuState::pause() {

}


void PauseMenuState::resume() {

}


void PauseMenuState::update(StateEngine* eng) {

}


void PauseMenuState::draw(StateEngine* eng) {
	eng->window.setView(eng->window.getDefaultView());
	eng->window.draw(p_circ);
}


void PauseMenuState::handle_events(StateEngine* eng) {
	sf::Event event;

	while (eng->window.pollEvent(event)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
			sf::Joystick::isButtonPressed(0, JoyStickButtons::START)) {
			eng->pop_state();
			while (eng->window.pollEvent(event));
			break;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
			sf::Joystick::isButtonPressed(0, JoyStickButtons::B)) {
			eng->clear_states();
			eng->push_state(MainMenu::instance());
			while (eng->window.pollEvent(event));
			break;
		}

		/*
		switch (event.type) {
		case sf::Event::KeyPressed: {
			switch (event.key.code) {
			default:
				break;
			}
		}

		default:
			break;
		}
		*/
	}
}
