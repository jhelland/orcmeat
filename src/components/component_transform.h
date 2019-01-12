// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	Component for entity position
//


#ifndef COMPONENT_TRANSFORM_H
#define COMPONENT_TRANSFORM_H


#include <SFML/Graphics.hpp>

#include "../core/core.h"


class TransformComponent : public core::Component {  //core::ComponentBase<TransformComponent> {
private:
	static std::vector<id::IdType> owners;
	core::Entity* owner;
	
	sf::Vector2f position;
	float velocity;

public:
	TransformComponent() : position(sf::Vector2f(0.0, 0.0)), velocity(0.05) {}
	TransformComponent(sf::Vector2f position) : position(position), velocity(0.05) {}
	TransformComponent(sf::Vector2f position, float velocity) : position(position), velocity(velocity) {}

	inline sf::Vector2f get_position() const { return position; }
	inline void set_position(sf::Vector2f position) { this->position = position; }
	inline void update_position(sf::Vector2f& direction) { this->position += velocity * direction; }

	inline void set_owner(core::Entity* owner) { this->owner = owner; }
};


#endif // !COMPONENT_TRANSFORM_H
