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
#include "entity_circle.h"


class GameState : public State {
public:
	void initialize();
	void clean();

	void pause();
	void resume();

	void update(StateEngine*);
	void draw(StateEngine*);
	void handle_events(StateEngine*);
	static GameState* instance() { return &g_state; } // allow for convenient switching between states

	int get_entities_length() const { return entities.size(); }
	void add_entity(Entity* entity);
	Entity* get_entity_by_id(unsigned int id) const;
	void clear_entities();

	std::vector<unsigned int> entitiesDist0;		// An example of a list of entities w/in update range

protected:
	GameState() { }

private:
	static GameState g_state; // Static to prevent creating new copies with every instance of the state
	std::unordered_map<unsigned int, Entity*> entities;
	sf::Texture background_img;
	sf::Sprite background;
};


#endif 
