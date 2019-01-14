// Create by jhelland (12/21/18)
//
// DESCRIPTION:
//  State manager for the main menu
//


#include <iostream>

#include "globals.h"
#include "main_menu_state.h"


MainMenu MainMenu::m_menu;


void MainMenu::initialize() {
	playButton_t.loadFromFile("textures\\flames4.jpg"); //"../textures/flames4.jpg");
	playButton.setTexture(playButton_t);
}


void MainMenu::clean() {

}


void MainMenu::pause() {

}


void MainMenu::resume() {

}


void MainMenu::update(StateEngine* eng) {

}


void MainMenu::draw(StateEngine* eng) {
	eng->window.setView(eng->window.getDefaultView());
	eng->window.draw(playButton);
}


void MainMenu::handle_events(StateEngine* eng) {
	sf::Event event;
	while (eng->window.pollEvent(event)) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
			sf::Joystick::isButtonPressed(0, JoyStickButtons::A)) {
			eng->change_state(GameState::instance());
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
			sf::Joystick::isButtonPressed(0, JoyStickButtons::B)) {
			eng->exit();
		}

		switch (event.type) {
		// Handle manually closing window
		case sf::Event::Closed:
			eng->exit();
			break;

		default:
			break;
		}
	}
}
