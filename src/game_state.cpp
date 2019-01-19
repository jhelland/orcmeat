// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main game state. Encapsulates sound, music, and handles player input.
//


#include <math.h>  // sqrt
#include <unordered_set>
#include <random>

#ifdef _DEBUG
#include<iostream>
#endif

#include "globals.h"
#include "utils/util_functions.h"
#include "game_state.h"
#include "sound.h"
#include "music.h"
#include "entities/entity_circle.h"
#include "entities/entity_rectangle.h"


inline float dot_product(const sf::Vector2f& p1, const sf::Vector2f& p2) { return p1.x*p2.x + p1.y*p2.y; }

inline float norm(const sf::Vector2f& v) { return std::sqrt(dot_product(v, v)); }


sf::Vector2f get_player_direction() {
	sf::Vector2f direction{};
	bool joyStickFlag = sf::Joystick::isConnected(0);  // joystick slot 0

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		direction.y -= 1.f;
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		direction.x -= 1.f;
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		direction.y += 1.f;
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		direction.x += 1.f;
		joyStickFlag = false;
	}

	if (joyStickFlag) {
		float deadZone = 0.1f;
		sf::Vector2f stickInput(0.f, 0.f);
		stickInput.x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		stickInput.y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		float nrm = norm(stickInput);

		// Use a scaled radial deadzone: http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html
		if (nrm / 100 < deadZone) {
			stickInput.x = stickInput.y = 0;
		}
		else {
			stickInput = (stickInput / (nrm)) * ((nrm / 100 - deadZone) / (1 - deadZone));
		}
		direction = stickInput;
	}

	return direction;
}


GameState GameState::g_state; // forward declaration for static GameState g_state


void GameState::initialize() {
	// background image
	background_img.loadFromFile("textures\\background.jpg");  // linux, mac: "../textures/background.jpg");
	background.setTexture(background_img);

	// continuous background music
	msc::play_music("music\\Induction.ogg");  // linux, mac: "../music/Induction.ogg");	

	// Create collidable rectangles
	core::Entity* wall = entityManager.add_entity<RectangleEntity>(100.f, 100.f, 10.f, 400.f, sf::Color::Black);  // left wall
	entitiesDist0.push_back(wall->get_id());	

	// Initialize player in entity list
	core::Entity* player = entityManager.add_entity<CircleEntity>(400.f, 300.f, 10.f, sf::Color::Magenta);
	entitiesDist0.push_back(player->get_id());
	playerId = player->get_id();

	// Initialize camera
	camera.set_position(player->get_position());
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
	/*
	for (auto& id : entitiesToPurgeList) {
		entityManager.delete_entity(id);
	}
	entitiesToPurgeList.clear();
	*/

	auto player = entityManager.get_entity_by_id(playerId);

	// Slow player velocity with time
	float decay = 0.5;
	player->velocity -= (2.f*decay - decay*decay) * player->velocity;

	// Player movement
	float speed = 170.f;
	float deltaTime = eng->get_delta_time();
	player->velocity += speed * get_player_direction();
	
	// Collisions: no exhaustive collision checking yet, just collisions with the player
	qt_create(&collisionTree, 800, 600, 8, 8);
	std::unordered_map<int, id::IdType> collisionIdMap;  // IDs in quadtree are not the same as the UUIDs
	for ( auto& it : *entityManager.get_entity_register() ) {
		//if ( it.first != playerId) {  // NOCLIP BABYYY
		auto box = it.second->get_bounding_box();
		auto collId = qt_insert(&collisionTree, it.first, box.left, box.top, box.left + box.width, box.top + box.height);
		collisionIdMap[collId] = it.first;
		//}
	}	

	// Resolve possible collisions.
	// Iterate over each entity in world, search quadtree for possible colliders, resolve for each collider.
	// Key: don't update positions/velocities directly here
	std::unordered_map<id::IdType, sf::Vector2f> velocityUpdates;
	IntList intList; il_create(&intList, 0);
	sf::Rect<float> worldRect(0.f, 0.f, 800.f, 600.f);
	std::vector<sf::Color> colors{ sf::Color::Red, sf::Color::Blue, sf::Color::Black, sf::Color::Magenta, sf::Color::Green, sf::Color::Cyan, sf::Color::Yellow };
	bool playerCollideFlag = false;
	for ( auto& it : *entityManager.get_entity_register() ) {
		//if (it.first == playerId) continue;  // NOCLIP BABYYYYYYYYY

		auto futurePosition = it.second->get_position() + deltaTime * it.second->velocity;
		auto futureBox = it.second->get_bounding_box();	
		futureBox.left = futurePosition.x;
		futureBox.top = futurePosition.y;

		qt_query(&collisionTree, &intList, futureBox.left, futureBox.top, futureBox.left + futureBox.width, futureBox.top + futureBox.height, -1);
		for ( int n = 0; n < il_size(&intList); ++n ) {
			auto collId = collisionIdMap[ il_get(&intList, n, 0) ];
			auto other = entityManager.get_entity_by_id(collId);
			auto otherFuturePosition = other->get_position() + deltaTime * other->velocity;
			auto otherFutureBox = other->get_bounding_box();
			otherFutureBox.left = otherFuturePosition.x;
			otherFutureBox.top = otherFuturePosition.y;

			if ( collId != it.first && futureBox.intersects(otherFutureBox) ) {
				//dynamic_cast<CircleEntity*>(other)->set_color(colors[rand() % colors.size()]);
				//dynamic_cast<CircleEntity*>(it.second)->set_color(colors[rand() % colors.size()]);

				auto diff = other->velocity - it.second->velocity;
				diff /= norm(diff);
				velocityUpdates[collId] = -diff * norm(other->velocity);
				velocityUpdates[it.first] = diff * norm(it.second->velocity);

				// GET OUT OF THERE YOU STUPID CIRCLE
				// This loop isn't optimal, the correct movement distance could be calculated.
				while ( futureBox.intersects(otherFutureBox) ) {
					other->move(0.01f * (velocityUpdates[collId]));
					it.second->move(0.01f * velocityUpdates[it.first]);
					futureBox = it.second->get_bounding_box();
					otherFutureBox = other->get_bounding_box();
				}

				if ( it.first == playerId ) {
					camera.start_screen_shake();
				}
			}
		}

		if ( !worldRect.contains(futureBox.left, futureBox.top) || !worldRect.contains(futureBox.left + futureBox.width, futureBox.top + futureBox.height) ) {
			velocityUpdates[it.first] = -it.second->velocity;
			
			// Need to figure out which wall collision occurred on for sake of normal vector
			float x1 = futureBox.left;
			float y1 = futureBox.top;
			if (x1 < worldRect.left) {
				x1 += worldRect.left - x1;
			}
			if (y1 < worldRect.top) {
				y1 += worldRect.top - y1;
			}
			if (x1 + futureBox.width > worldRect.left + worldRect.width) {
				x1 -= (x1 + futureBox.width) - (worldRect.left + worldRect.width);
			}
			if ((y1 + futureBox.height) > (worldRect.top + worldRect.height)) {
				y1 -= (y1 + futureBox.height) - (worldRect.top + worldRect.height);
			}
			it.second->set_position(sf::Vector2f(x1, y1));
		}
	}

	// Update velocity, position after doing collision resolution
	for (auto& it : *entityManager.get_entity_register()) {
		if (velocityUpdates.count(it.first)) {
			it.second->velocity = velocityUpdates[it.first];
		}
		it.second->move(deltaTime * it.second->velocity);
	}

	qt_destroy(&collisionTree);

	// Update camera position and effects
	auto newCameraPos = player->get_position() + 20 * deltaTime * player->velocity;
	newCameraPos = (0.95f * newCameraPos + 0.05f *eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window)));
	camera.lerp_position(newCameraPos);
	camera.update(eng->playerView);
	eng->playerView.setCenter(camera.get_position());
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
			} else {
				entity->draw(eng->window);
			}
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
				for (int i = 0; i < 10; ++i) {
					auto r = std::max(4, (int)(10*(utils::rand() + 1) / 2));
					sf::Vector2i size(r, r);
					auto v = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window) - size);

					core::Entity* circEnt = entityManager.add_entity<CircleEntity>(v.x, v.y, (float)r, sf::Color::Red);
					circEnt->velocity = sf::Vector2f(170.f * utils::rand(), 170.f * utils::rand());

					entitiesDist0.push_back(circEnt->get_id());
				}
				//snd::play_sound(snd::blip, circEnt->get_position(), 100.f);
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
