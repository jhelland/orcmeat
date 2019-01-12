// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main game state. Encapsulates sound, music, and handles player input.
//


#include<iostream>

#include "game_state.h"
#include "sound.h"
#include "music.h"
#include "entities/entity_circle.h"
#include "components/component_transform.h"


GameState GameState::g_state; // forward declaration for static GameState g_state


void GameState::initialize() {
	// background image
	background_img.loadFromFile("textures\\background.jpg");  // linux, mac: "../textures/background.jpg");
	background.setTexture(background_img);

	// continuous background music
	msc::play_music("music\\Induction.ogg");  // linux, mac: "../music/Induction.ogg");

	// Initialize player in entity list
	core::Entity* player = entityManager.add_entity<CircleEntity>(400.f, 300.f, 30.f, sf::Color::Magenta);
	player->register_component<TransformComponent>();
	player->get_component<TransformComponent>()->set_position(player->get_position());

	entitiesDist0.push_back(player->get_id());
}


void GameState::clean() {
	msc::stop_music();
	id::reset();  // Reset UUID generator
	entityManager.clear_entities();
}


void GameState::pause() { 
	msc::pause_music();
}


void GameState::resume() {
	msc::resume_music();
}


void GameState::update(StateEngine* eng) {
	for (auto& id : entitiesPurgeList)
		entityManager.delete_entity(id);
	entitiesPurgeList.clear();

	// Update entity positions
	for (auto& it : *entityManager.get_entity_register()) {
		if (it.first == playerId) {
			// lerp between mouse position and player position
			auto position = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window));
			position -= eng->window.mapPixelToCoords(sf::Vector2i(30, 30));
			it.second->set_position(it.second->get_position() + 0.05f*(position - it.second->get_position()));
		}
		else if (it.second->has_component<TransformComponent>()) {
			// lerp between mouse click point and random point
			auto position = it.second->get_component<TransformComponent>()->get_position();
			it.second->set_position(it.second->get_position() + 0.01f*(position - it.second->get_position()));

			// Update purge list by checking collision with edge of window
			auto winDims = eng->window.mapPixelToCoords(eng->get_window_dimensions());
			if (!sf::Rect<float>::Rect(-40.f, -40.f, winDims.x, winDims.y).contains(it.second->get_position())) {
				entitiesPurgeList.push_back(it.first);
			}
		}
	}
} 


void GameState::draw(StateEngine* eng) {
	eng->window.setView(eng->playFieldView);
	eng->window.clear(sf::Color(0, 0, 0, 255));
	eng->window.draw(background);

	for (auto& id : entitiesDist0) {
		core::Entity* entity = entityManager.get_entity_by_id(id);
		if (entity != nullptr) {
			if (entity->get_id() == playerId) {
				eng->window.setView(eng->window.getDefaultView());
				entity->draw(eng->window);
				eng->window.setView(eng->playFieldView);
			}
			else
				entity->draw(eng->window);
		}
	}
}


void GameState::handle_events(StateEngine *eng) {
	sf::Event event;
	while (eng->window.pollEvent(event)) {
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
				// Create new entity
				sf::Vector2f v = eng->window.mapPixelToCoords(sf::Vector2i(20, 20));  // Account for circle radius
				v = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window)) - v;

				core::Entity* circEnt = entityManager.add_entity<CircleEntity>(v.x, v.y, 20.f, sf::Color::Red);
				circEnt->register_component<TransformComponent>();
				circEnt->get_component<TransformComponent>()->set_position(sf::Vector2f(rand()%600*2.f, rand()%600*2.f));

				entitiesDist0.push_back(circEnt->get_id());

				snd::play_sound(snd::blip, circEnt->get_position(), 100.f);
				break;
			}	

			case sf::Mouse::Right: {

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
			eng->exit();  // Sets running flag to false
			break;

		default:
			break;
		}
	}
}
