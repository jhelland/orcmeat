// Created by jhelland (1/6/19)
//
// DESCRIPTION:
//	Generic entity class with unique ID per instance.
//


#ifndef ENTITY_H
#define ENTITY_H


#include <iostream>

#include <SFML/Graphics.hpp>

#include "id_gen.h"  // For generating unique ID for each entity instance


class Entity {
public:
	Entity() : id(id::generate_id()) {}

	unsigned int get_id() const { return id; }
	virtual sf::Vector2f get_position() const = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

private:
	const unsigned int id;  // Unique ID for indexing in global entity table
};


#endif