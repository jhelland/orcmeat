// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	An entity for the sake of testing the engine.
// 


#ifndef ENTITY_CIRCLE_H
#define ENTITY_CIRCLE_H


#include <vector>
#include <random>
#include <math.h>

#include <SFML/Graphics.hpp>

#include "../ecs/entity.h"


static std::random_device rd;
static std::mt19937_64 generator(rd());
static std::uniform_real_distribution<> distribution(-30.f, 30.f);


class CircleEntity : public ecs::Entity {  
private:
	sf::CircleShape circle;
	sf::Vector2f direction;

public:
	CircleEntity(float x, float y, float radius = 1.0, sf::Color color = sf::Color::Red) {
		circle.setFillColor(color);
		circle.setPosition(x, y);
		circle.setRadius(radius);

		direction.x = distribution(generator);
		direction.y = distribution(generator);
		direction /= std::sqrt(direction.x*direction.x + direction.y*direction.y);
	}

	inline sf::CircleShape get_circ() const { return circle; }
	inline void set_color(sf::Color color) { circle.setFillColor(color); }
	inline sf::Vector2f get_direction() const { return direction; }
	inline sf::Vector2f get_position() const { return circle.getPosition(); }
	inline void set_position(sf::Vector2f position) { circle.setPosition(position); }
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const { target.draw(circle); }
};


#endif