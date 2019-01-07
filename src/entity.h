// Created by jhelland (1/6/19)
//
// DESCRIPTION:
//	General entity class
//


#ifndef ENTITY_H
#define ENTITY_H


#include <iostream>

#include <uuid.h>
#include <SFML/Graphics.hpp>

// Generate unique IDs for each entity. This should be static to avoid reinitializing the
// Marsenne Twister engine implicit in the object for every entity created.
static uuids::uuid_random_generator uuid_gen;  


// I'm including the UUID generation here because I want to make it as brainless as possible
// to create a new entity and insert it into the global entities table.
class Entity {
public:
	Entity() : id(uuid_gen()) {}

	uuids::uuid get_id() { return id; }
	virtual sf::Vector2f get_position() = 0;

private:
	const uuids::uuid id;  // Unique ID for indexing in global entity table
};


#endif