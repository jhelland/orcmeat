// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main game state. Encapsulates sound, music, and handles player input.
//


#include<iostream>

#include "game_state.h"
#include "sound.h"
#include "music.h"
#include "entity_circle.h"


GameState GameState::g_state; // declaration for static GameState g_state


void GameState::initialize() {
	// background image
	background_img.loadFromFile("textures\\background.jpg");  // linux, mac: "../textures/background.jpg");
	background.setTexture(background_img);

	// background music
	msc::play_music("music\\Induction.ogg");  // linux, mac: "../music/Induction.ogg");
}


void GameState::clean() {
	// stop playing music
	msc::stop_music();
}


void GameState::pause() { 
	msc::pause_music();
}


void GameState::resume() {
	msc::resume_music();
}

void GameState::update(StateEngine* eng) {

}

using namespace std;
void GameState::draw(StateEngine* eng) {
	eng->window.setView(eng->playFieldView);
	eng->window.clear(sf::Color(0, 0, 0, 255));
	eng->window.draw(background);

	for (auto& id : eng->entitiesDist0) {
		Entity* entity = eng->get_entity_by_id(id);
		if (entity != nullptr)
			eng->window.draw(*dynamic_cast<CircleEntity*>(entity));
	}

	// Test to make sure view is working properly by drawing magenta circle
	eng->window.setView(eng->window.getDefaultView());
	sf::CircleShape c(30.f, 30);
	c.setFillColor(sf::Color::Magenta);
	c.setPosition(400.f, 300.f);
	eng->window.draw(c);
}

void GameState::handle_events(StateEngine *eng) {
	sf::Event event;
	while (eng->window.pollEvent(event)) {
		std::cout << "Game\t" << event.key.code << "\t" << eng->get_stack_length() << std::endl;
		switch (event.type) {
		case sf::Event::KeyPressed: {
			switch (event.key.code) {
			case sf::Keyboard::Escape:
				pause();
				eng->push_state(PauseMenuState::instance());
				break;

			case sf::Keyboard::Space:
				break;

			case sf::Keyboard::BackSpace:
				break;

			case sf::Keyboard::A:
				break;

			default:
				break;
			}
			break;
		}

		case sf::Event::MouseButtonPressed: {
			switch (event.key.code) {
			case sf::Mouse::Left: {
				sf::Vector2f v = eng->window.mapPixelToCoords(sf::Vector2i(20, 20));
				v = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window)) - v;
				Entity* circEnt = new CircleEntity(v.x, v.y, 20.0, sf::Color::Red);

				// Update global entity list with new entity
				eng->add_entity(circEnt);

				snd::play_sound(snd::blip, circEnt->get_position(), 100.f);
				break;
			}

			default:
				break;
			}
			break;
		}

		// If window is minimized or something, pause game
		case sf::Event::LostFocus:
			pause();
			eng->push_state(PauseMenuState::instance());
			break;

		case sf::Event::Closed:
			eng->exit();
			break;

		default:
			break;
		}
	}
}
