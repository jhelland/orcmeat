// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  State manager for the main game.
//


#ifndef GAME_H
#define GAME_H


#include <vector>
#include <unordered_map>
#include <math.h>  // sqrt

#ifdef _DEBUG
#include<iostream>
#endif

#include <SFML/Graphics.hpp>

#include "globals.h"
#include "utils/util_functions.h"
#include "sound.h"
#include "music.h"

#include "main_menu_state.h"
#include "pause_menu_state.h"

#include "core/entity_manager.h"

#include "utils/id_gen.h"
#include "utils/data_structures/quadtree.h"

#include "entities/camera.h"


class GameState final : public State {
private:
	static GameState g_state;  // Static instance of self for initialization on state stack

	sf::Texture background_img;
	sf::Sprite background;
	id::IdType playerId;

	Quadtree collisionTree;
	Camera camera;

private:
	std::vector<std::pair<id::IdType, id::IdType>> get_collisions();

public:
	std::vector<id::IdType> entitiesDist0;		// An example of a list of entities w/in update range
	std::vector<id::IdType> entitiesToPurgeList;  
	core::EntityManager entityManager;			// Manages allocation of memory for entities and stuff

public: 
	// State management
	void initialize();
	void clean();

	void pause();
	void resume();

	void update(StateEngine*);
	void draw(StateEngine*);
	void handle_events(StateEngine*);
	inline static GameState* instance() { return &g_state; }  // allow for convenient switching between states

protected:
	GameState() {}
};


#endif 
