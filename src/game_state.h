// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  State manager for the main game.
//


#ifndef GAME_H
#define GAME_H


#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "main_menu_state.h"
#include "pause_menu_state.h"
#include "entities/entity_circle.h"
#include "utils/id_gen.h"


class GameState : public State {
private:
	static GameState g_state; // Static to prevent creating new copies with every instance of the state
	std::unordered_map<id::EntityIdType, ecs::Entity*> entitiesRegister;
	sf::Texture background_img;
	sf::Sprite background;
	id::EntityIdType playerId;
	id::EntityIdType transformId;

public:
	std::vector<id::EntityIdType> entitiesDist0;		// An example of a list of entities w/in update range
	std::vector<id::EntityIdType> entitiesPurgeList;

public:
	// State management
	void initialize();
	void clean();

	void pause();
	void resume();

	void update(StateEngine*);
	void draw(StateEngine*);
	void handle_events(StateEngine*);
	inline static GameState* instance() { return &g_state; } // allow for convenient switching between states

	// Entity management
	inline int get_entities_length() const { return entitiesRegister.size(); }
	void register_entity(ecs::Entity* entity);
	inline ecs::Entity* get_entity_by_id(id::EntityIdType id) const { return has_entity(id) ? entitiesRegister.at(id) : nullptr; }
	inline bool has_entity(id::EntityIdType id) const { return (bool)entitiesRegister.count(id); }
	void delete_entity(id::EntityIdType id);
	void clear_entities();

protected:
	GameState() {}
};


#endif 
