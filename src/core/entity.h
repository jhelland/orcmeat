// Created by jhelland (1/6/19)
//
// DESCRIPTION:
//	Generic entity class with unique ID per instance.
//


#ifndef ENTITY_H
#define ENTITY_H


#include <unordered_map>
#include <typeinfo>

#include <SFML/Graphics.hpp>

#include "../utils/id_gen.h"  


namespace core {

	class Entity {
	protected:
		const id::IdType id;  // Instance-unique ID for indexing in global entity table

	public:
		Entity() : id(id::generate_id()) {}
	
		id::IdType get_id() const { return id; }

		virtual sf::Rect<float> get_bounding_box() const = 0;

		virtual sf::Vector2f get_position() const = 0;

		virtual void set_position(sf::Vector2f position) = 0;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;
	};  // class Entity

}  // namespace core


#endif