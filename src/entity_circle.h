// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	An entity for the sake of testing the engine.
// 


#ifndef ENTITY_CIRCLE_H
#define ENTITY_CIRCLE_H


#include <vector>

#include <SFML/Graphics.hpp>

#include "entity.h"


class CircleEntity : public Entity {  //, public sf::Drawable {
public:
	CircleEntity(float x, float y, float radius = 1.0, sf::Color color = sf::Color::Red) {
		circle.setFillColor(color);
		circle.setPosition(x, y);
		circle.setRadius(radius);
	}
	sf::CircleShape get_circ() const { return circle; }
	sf::Vector2f get_position() const { return circle.getPosition(); }
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const { target.draw(circle); }

private:
	sf::CircleShape circle;
};


#endif