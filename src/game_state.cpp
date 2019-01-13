// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main game state. Encapsulates sound, music, and handles player input.
//


#include <math.h>
#ifdef _DEBUG
#include<iostream>
#endif

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
	core::Entity* player = entityManager.add_entity<CircleEntity>(400.f, 300.f, 10.f, sf::Color::Magenta);
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

	// Player movement
	// TODO: extract to separate function
	float deltaTime = eng->get_delta_time();
	float velocity = 120.f;
	auto player = entityManager.get_entity_by_id(playerId);
	auto position = player->get_position();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		position -= deltaTime * sf::Vector2f(0.f, velocity);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		position -= deltaTime * sf::Vector2f(velocity, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		position += deltaTime * sf::Vector2f(0.f, velocity);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		position += deltaTime * sf::Vector2f(velocity, 0.f);
	}
	player->set_position(position);

	// Camera tracking
	// Smooth interpolation -- camera adjusts based on mouse position
	auto mousePos = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window), eng->playerView);
	auto diff = mousePos - (position + sf::Vector2f(10.f, 10.f));
	float norm = std::sqrt(diff.x*diff.x + diff.y*diff.y);
	float bound = 800.f;
	std::cout << eng->window.mapCoordsToPixel(mousePos).x << " " << mousePos.y << std::endl;
	if (sf::Rect<int>(0, 0, 800, 600).contains(eng->window.mapCoordsToPixel(mousePos))) {
		if (norm < bound) {
			float interp = norm / bound;
			diff.y *= 1.5;  // Account for aspect ratio
			position += interp * interp * diff;
		}
		else {
			position += diff / norm * bound;
		}
	}
	eng->playerView.setCenter(position);

	// Update entity positions
	for (auto& it : *entityManager.get_entity_register()) {
		if (it.first != playerId && it.second->has_component<TransformComponent>()) {
			// lerp between mouse click point and random point
			auto position = it.second->get_component<TransformComponent>()->get_position();
			it.second->set_position(it.second->get_position() + 0.01f*(position - it.second->get_position()));

			// Update purge list by checking collision with edge of window
			auto winDims = eng->window.mapPixelToCoords(eng->get_window_dimensions());
			if (!sf::Rect<float>::Rect(0.f, 0.f, winDims.x, winDims.y).contains(it.second->get_position())) {
				entitiesPurgeList.push_back(it.first);
			}
		}
	}
} 


void GameState::draw(StateEngine* eng) {
	// Draw background world stuff
	eng->window.setView(eng->window.getDefaultView());
	eng->window.setView(eng->playerView);
	eng->window.clear(sf::Color(0, 0, 0, 255));
	eng->window.draw(background);

	// Draw player view stuff
	eng->window.setView(eng->playerView);
	for (auto& id : entitiesDist0) {
		core::Entity* entity = entityManager.get_entity_by_id(id);
		if (entity != nullptr) {
			if (entity->get_id() == playerId) {
				entity->draw(eng->window);
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

			default:
				break;
			}
			break;
		}

		case sf::Event::MouseButtonPressed: {
			switch (event.key.code) {
			case sf::Mouse::Left: {
				// Create new entity
				//sf::Vector2f v = eng->window.mapPixelToCoords(sf::Vector2i(5, 5));  // Account for circle radius
				auto v = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window) - sf::Vector2i(5, 5));		

				core::Entity* circEnt = entityManager.add_entity<CircleEntity>(v.x, v.y, 5.f, sf::Color::Red);
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
