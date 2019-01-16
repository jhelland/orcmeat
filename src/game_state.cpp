// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main game state. Encapsulates sound, music, and handles player input.
//


#include <math.h>  // sqrt
#include <unordered_set>
#ifdef _DEBUG
#include<iostream>
#endif

#include "globals.h"
#include "utils/util_functions.h"
#include "game_state.h"
#include "sound.h"
#include "music.h"
#include "entities/entity_circle.h"


GameState GameState::g_state; // forward declaration for static GameState g_state


void GameState::initialize() {
	// background image
	background_img.loadFromFile("textures\\background.jpg");  // linux, mac: "../textures/background.jpg");
	background.setTexture(background_img);

	// continuous background music
	msc::play_music("music\\Induction.ogg");  // linux, mac: "../music/Induction.ogg");

	// Initialize player in entity list
	core::Entity* player = entityManager.add_entity<CircleEntity>(400.f, 300.f, 10.f, sf::Color::Magenta);

	// Put player in distance category 0 (category is centered around player)
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
	// TODO: extract to separate function?
	float deltaTime = eng->get_delta_time();
	float velocity = 170.f;
	auto player = entityManager.get_entity_by_id(playerId);
	auto position = player->get_position();
	bool joyStickFlag = sf::Joystick::isConnected(0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		position -= deltaTime * sf::Vector2f(0.f, velocity);
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		position -= deltaTime * sf::Vector2f(velocity, 0.f);
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		position += deltaTime * sf::Vector2f(0.f, velocity);
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		position += deltaTime * sf::Vector2f(velocity, 0.f);
		joyStickFlag = false;
	}
	
	sf::Vector2f stickInput(0.f, 0.f);
	if (joyStickFlag) {
		float deadZone = 0.1f;
		stickInput.x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		stickInput.y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		float norm = std::sqrt(stickInput.x*stickInput.x + stickInput.y * stickInput.y);

		// Use a scaled radial deadzone: http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html
		if (norm / 100 < deadZone)
			stickInput.x = stickInput.y = 0;
		else
			stickInput = (stickInput / (norm)) * ((norm / 100 - deadZone) / (1 - deadZone));

		position += deltaTime * velocity * stickInput;

		// Camera tracking with joystick
		float interp = norm / 100;
		auto cameraPos = position + interp * interp * interp * stickInput * deltaTime * 100.f;
		eng->playerView.setCenter(cameraPos);
	}

	player->set_position(position);

	// Camera tracking with mouse
	// Smooth interpolation -- camera adjusts based on mouse position
	//if (!joyStickFlag) {
		auto mousePos = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window), eng->playerView);
		auto diff = mousePos - (position + sf::Vector2f(10.f, 10.f));
		float norm = std::sqrt(diff.x*diff.x + diff.y*diff.y);
		float bound = 800.f;

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
	//}

	// Update entity positions
		/*
	for (auto& it : *entityManager.get_entity_register()) {
		// Update purge list by checking collision with edge of current view
		auto viewBox = eng->get_view_box(eng->playerView);
		if (!viewBox.contains(it.second->get_position())) {
			entitiesPurgeList.push_back(it.first);
		}
	}
	*/

	// Collisions: no exhaustive collision checking yet, just collisions with the player
	//Quadtree<id::IdType, float> collisionQuadtree(0, sf::Rect<float>(0.f, 0.f, 800.f, 600.f));
	qt_create(&collisionTree, 800, 600, 8, 8);
	std::unordered_map<int, id::IdType> collisionIdMap;  // IDs in quadtree are not the same as the UUIDs

	for (auto& it : *entityManager.get_entity_register()) {
		if (it.first != playerId) {
			auto box = it.second->get_bounding_box();
			auto collId = qt_insert(&collisionTree, it.first, box.left, box.top, box.left + box.width, box.top + box.height);
			collisionIdMap[collId] = it.first;
		}
	}

	auto box = entityManager.get_entity_by_id(playerId)->get_bounding_box();
	IntList intList; il_create(&intList, 0);

	qt_query(&collisionTree, &intList, box.left, box.top, box.left + box.width, box.top + box.height, -1);
	for (int n = 0; n < il_size(&intList); ++n) {
		auto collId = il_get(&intList, n, 0);
		dynamic_cast<CircleEntity*>(entityManager.get_entity_by_id(collisionIdMap[collId]))->set_color(sf::Color::Black);
	}

	//il_destroy(&intList);
	qt_destroy(&collisionTree);
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
		// Pause game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
			sf::Joystick::isButtonPressed(0, JoyStickButtons::START)) {
			pause();
			eng->push_state(PauseMenuState::instance());
		}

		switch (event.type) {
		case sf::Event::MouseButtonPressed: {
			switch (event.key.code) {
			case sf::Mouse::Left: {
				// Create new entity
				//sf::Vector2f v = eng->window.mapPixelToCoords(sf::Vector2i(5, 5));  // Account for circle radius
				auto r = std::max(5, rand() % 100);
				sf::Vector2i size(r, r);
				auto v = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window) - size);		

				core::Entity* circEnt = entityManager.add_entity<CircleEntity>(v.x, v.y, (float)r, sf::Color::Red);

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
