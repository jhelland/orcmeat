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


GameState GameState::g_state; // declaration for static GameState g_state


void GameState::initialize() {
	// background image
	background_img.loadFromFile("textures\\background.jpg");  // linux, mac: "../textures/background.jpg");
	background.setTexture(background_img);

	// background music
	msc::play_music("music\\Induction.ogg");  // linux, mac: "../music/Induction.ogg");

	// Initialize player in entity list
	ecs::Entity* player = new CircleEntity(400.f, 300.f, 30.f, sf::Color::Magenta);
	playerId = player->get_id();
	ecs::Component* transform = new TransformComponent(player->get_position());
	transformId = transform->get_id();
	player->register_component(transform);
	register_entity(player);
}


void GameState::clean() {
	// stop playing music
	msc::stop_music();
	id::reset();  // Reset UUID generator
	clear_entities();
}


void GameState::pause() { 
	msc::pause_music();
}


void GameState::resume() {
	msc::resume_music();
}


void GameState::update(StateEngine* eng) {
	// Delete entities in purge list
	for (auto& id : entitiesPurgeList)
		delete_entity(id);
	entitiesPurgeList.clear();

	// Update player position
	sf::Vector2f v = eng->window.mapPixelToCoords(sf::Mouse::getPosition(eng->window));
	v -= eng->window.mapPixelToCoords(sf::Vector2i(30, 30));
	v -= entitiesRegister[playerId]->get_position();
	dynamic_cast<TransformComponent*>(entitiesRegister[playerId]->get_component_by_id(transformId))->update_position(v);
	sf::Vector2f newPos = dynamic_cast<TransformComponent*>(entitiesRegister[playerId]->get_component_by_id(transformId))->get_position();
	auto winDims = eng->window.mapPixelToCoords(eng->get_window_dimensions());

	if (!sf::Rect<float>::Rect(0.f, 0.f, winDims.x - 60.f, winDims.y - 60.f).contains(newPos)) {
		newPos = entitiesRegister[playerId]->get_position();
		dynamic_cast<TransformComponent*>(entitiesRegister[playerId]->get_component_by_id(transformId))->update_position(-v);
	}

	dynamic_cast<CircleEntity*>(entitiesRegister[playerId])->set_position(newPos);


	// Update other entity positions
	for (auto& it : entitiesRegister) {
		if (it.first != playerId && it.second->has_component(transformId)) {
			sf::Vector2f direction = dynamic_cast<CircleEntity*>(it.second)->get_direction();
			dynamic_cast<TransformComponent*>(it.second->get_component_by_id(transformId))->update_position(direction);
			newPos = dynamic_cast<TransformComponent*>(it.second->get_component_by_id(transformId))->get_position();
			dynamic_cast<CircleEntity*>(it.second)->set_position(newPos);
		
			// Update entities to be deleted
			sf::Vector2f position = it.second->get_position();
			if (!sf::Rect<float>::Rect(-40.f, -40.f, 40.f + winDims.x, 40.f + winDims.y).contains(position)) {
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
		ecs::Entity* entity = get_entity_by_id(id);
		if (entity != nullptr) {
			//eng->window.draw(*dynamic_cast<CircleEntity*>(entity));
			if (entity->get_id() == playerId) {
				// Draw player in a different view
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
				float velocity = distribution(generator) / 10.f;
				unsigned int thresh = 255 - (int)std::abs(255 * velocity);
				ecs::Entity* circEnt = new CircleEntity(v.x, v.y, 20.0, sf::Color::Color(128, 20, thresh, 255));
				ecs::Component* transform = new TransformComponent(circEnt->get_position(), velocity);

				// Update global entity list with new entity
				circEnt->register_component(transform);
				register_entity(circEnt);

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


void GameState::register_entity(ecs::Entity* entity) {
	entitiesRegister.insert({ entity->get_id(), entity });  // Update global entity list
	entitiesDist0.push_back(entity->get_id());      // Update list of enemies w/in range 0
}


void GameState::delete_entity(id::EntityIdType id) {
	if (has_entity(id)) {
		delete entitiesRegister.at(id);
		entitiesRegister.erase(id);
	}
}


void GameState::clear_entities() {
	// Clear heap allocated entities
	for (auto& it : entitiesRegister)
		delete it.second;

	// Remove entity pointers and IDs from lists
	entitiesRegister.clear();
	entitiesDist0.clear();
	entitiesDist0.resize(0);
}

