// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	Component for entity position
//


#ifndef COMPONENT_TRANSFORM_H
#define COMPONENT_TRANSFORM_H


#include "../ecs/component.h"

#include <SFML/Graphics.hpp>


class TransformComponent : public ecs::ComponentBase<TransformComponent> {
private:
	sf::Vector2f position;
	float velocity;

public:
	TransformComponent() : position(sf::Vector2f(0.0, 0.0)), velocity(0.05) {}
	TransformComponent(sf::Vector2f position) : position(position), velocity(0.05) {}
	TransformComponent(sf::Vector2f position, float velocity) : position(position), velocity(velocity) {}

	inline sf::Vector2f get_position() const { return position; }
	inline void update_position(sf::Vector2f& direction) { this->position += velocity * direction; }
};


#endif // !COMPONENT_TRANSFORM_H
