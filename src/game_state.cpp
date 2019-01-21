// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main game state. Encapsulates sound, music, and handles player input.
//


#include <unordered_set>

#include "game_state.h"
#include "entities/entity_rectangle.h"
#include "entities/entity_player.h"
#include "entities/entity_circle.h"
#include "utils/math_functions.h"


GameState GameState::g_state; // forward declaration for static GameState g_state


void GameState::initialize() {
	// background image
	background_img.loadFromFile("textures\\background.jpg");  // linux, mac: "../textures/background.jpg");
	background.setTexture(background_img);

	// continuous background music
	//msc::play_music("music\\Induction.ogg");  // linux, mac: "../music/Induction.ogg");

	// Create collidable rectangles
	/// WARNING: make sure pool allocator has appropriate chunk sizes before uncommenting this
	/*
	core::Entity* wall = entityManager.add_entity<RectangleEntity>(100.f, 100.f, 10.f, 400.f, sf::Color::Black);  // left wall
	entitiesDist0.push_back(wall->get_id());	
	*/

	core::Entity* test = entityManager.add_entity<CircleEntity>(200.f, 200.f, 40.f, sf::Color::Blue);
	entitiesDist0.push_back(test->get_id());

	// Initialize player in entity list
	core::Entity* player = entityManager.add_entity<PlayerEntity>(400.f, 300.f, 15.f, sf::Color::Magenta);
	entitiesDist0.push_back(player->get_id());
	playerId = player->get_id();

	// Initialize camera
	camera.set_position(player->get_position());
}


void GameState::clean() {
	//msc::stop_music();
	id::reset();  // Reset UUID generator
	entityManager.clear_entities();
	entitiesDist0.clear();
}


void GameState::pause() { 
	//msc::pause_music();
}


void GameState::resume() {
	//msc::resume_music();
}


void GameState::update(StateEngine* eng) {
	for (auto&& id : entitiesToPurgeList) {
		entityManager.delete_entity(id);
		for (int i = 0; i < entitiesDist0.size(); ++i) {
			if (entitiesDist0[i] == id) {
				entitiesDist0.erase(entitiesDist0.begin() + i);
			}
		}
	}
	entitiesToPurgeList.clear();	

	for (auto&& id : entitiesDist0) {
		auto ent = entityManager.get_entity_by_id(id);
		ent->plan(this);
	}

	auto collisionList = get_collisions();

	// Resolve detected collisions
	for (auto& collision : collisionList) {
		entityManager.get_entity_by_id(collision.first)->collide(this, collision.second);
		entityManager.get_entity_by_id(collision.second)->collide(this, collision.first);
	}

	// Actions for entities
	float deltaTime = eng->get_delta_time();
	for (auto id : entitiesDist0) {
		auto ent = entityManager.get_entity_by_id(id);
		ent->act(this, deltaTime);
	}

	// Update camera position and effects
	auto player = entityManager.get_entity_by_id(playerId);
	auto newCameraPos = player->get_position();
	newCameraPos.x += player->get_bounding_box().width / 2.f;
	newCameraPos.y += player->get_bounding_box().height / 2.f;
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
		if ((event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) ||
			sf::Joystick::isButtonPressed(0, JoyStickButtons::START)) {
			pause();
			eng->push_state(PauseMenuState::instance());
			while (eng->window.pollEvent(event));
			break;
		}

		switch (event.type) {
		case sf::Event::MouseButtonPressed: {
			switch (event.key.code) {
			case sf::Mouse::Left: {
				auto mp = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window));
				// Create new entity
				for (int i = 0; i < 100; ++i) {
					auto r = std::max(4, (int)(10*(utils::rand() + 1) / 2));
					sf::Vector2i size(r, r);
					auto v = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window) - size);

					core::Entity* circEnt = entityManager.add_entity<CircleEntity>(v.x, v.y, (float)r, sf::Color::Red);
					circEnt->velocity = sf::Vector2f(80.f * utils::rand(), 80.f * utils::rand());

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


std::vector<std::pair<id::IdType, id::IdType>> GameState::get_collisions() {
	// Build a quadtree of collidable entities.
	// Does not include world boundaries.
	qt_create(&collisionTree, 800, 600, 8, 8);
	std::unordered_map<int, id::IdType> collisionIdMap;  // IDs in quadtree are not the same as the UUIDs
	for (auto& it : entityManager.get_entity_register()) {
		auto box = it.second->get_bounding_box();
		auto collId = qt_insert(&collisionTree, it.first, box.left, box.top, box.left + box.width, box.top + box.height);
		collisionIdMap[collId] = it.first;
	}

	// Detect collisions.
	// Iterate over each entity in world, search quadtree for possible colliders, resolve for each collider.
	// Don't update positions/velocities directly here, wait until resolution step
	std::vector<std::pair<id::IdType, id::IdType>> collisionList;
	IntList intList; il_create(&intList, 0);
	sf::Rect<float> worldRect(0.f, 0.f, 800.f, 600.f);
	std::vector<sf::Color> colors{ sf::Color::Red, sf::Color::Blue, sf::Color::Black, sf::Color::Magenta, sf::Color::Green, sf::Color::Cyan, sf::Color::Yellow };
	bool playerCollideFlag = false;
	for (auto& it : entityManager.get_entity_register()) {
		//if (it.first == playerId) continue;  // NOCLIP BABYYYYYYYYY

		auto position = it.second->get_position();
		auto box = it.second->get_bounding_box();
		box.left = position.x;
		box.top = position.y;

		qt_query(&collisionTree, &intList, box.left, box.top, box.left + box.width, box.top + box.height, -1);
		for (int n = 0; n < il_size(&intList); ++n) {
			auto collId = collisionIdMap[il_get(&intList, n, 0)];

			auto other = entityManager.get_entity_by_id(collId);
			auto otherPosition = other->get_position();
			auto otherBox = other->get_bounding_box();
			otherBox.left = otherPosition.x;
			otherBox.top = otherPosition.y;

			if (collId != it.first && box.intersects(otherBox)) {
				collisionList.push_back(std::make_pair(it.first, collId));  // Build collision list to resolve

				// EAT THE CIRCLES
				if (it.first == playerId) {
					entitiesToPurgeList.push_back(collId);
				}
				else if (collId == playerId) {
					entitiesToPurgeList.push_back(it.first);
				}

				/*
				// Some physics-based code that might be useful later
				auto relVel = other->body->velocity - it.second->body->velocity;
				float velAlongNormal = math::dot_product(relVel, normal);
				if (velAlongNormal > 0) continue;
				float e = std::min(it.second->body->restitution, other->body->restitution);
				float scaling = -(1.f + e) * velAlongNormal;
				scaling /= it.second->body->inv_mass() + other->body->inv_mass();
				sf::Vector2f impulse = scaling * normal;
				velocityUpdates[it.first] = -it.second->body->inv_mass() * impulse;
				velocityUpdates[collId] = other->body->inv_mass() * impulse;
				*/

				// JUICY
				if (it.first == playerId || collId == playerId) {
					camera.start_screen_shake();
				}
			}
		}

		if (!worldRect.contains(box.left, box.top) || !worldRect.contains(box.left + box.width, box.top + box.height)) {
			if (it.first != playerId) {
				it.second->velocity *= -1.f;
			}

			// Need to figure out which wall collision occurred on for sake of normal vector
			float x1 = box.left;
			float y1 = box.top;
			if (x1 < worldRect.left) {
				x1 += worldRect.left - x1;
			}
			if (y1 < worldRect.top) {
				y1 += worldRect.top - y1;
			}
			if (x1 + box.width > worldRect.left + worldRect.width) {
				x1 -= (x1 + box.width) - (worldRect.left + worldRect.width);
			}
			if ((y1 + box.height) > (worldRect.top + worldRect.height)) {
				y1 -= (y1 + box.height) - (worldRect.top + worldRect.height);
			}
			it.second->set_position(sf::Vector2f(x1, y1));
		}
	}
	qt_destroy(&collisionTree);

	return collisionList;
}