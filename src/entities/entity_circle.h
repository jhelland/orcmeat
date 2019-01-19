// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	An entity for the sake of testing the engine.
// 


#ifndef ENTITY_CIRCLE_H
#define ENTITY_CIRCLE_H


#include <SFML/Graphics.hpp>

#include "../core/core.h"


class CircleEntity : public core::Entity {  
private:
	sf::CircleShape circle;

public:
	CircleEntity(float x, float y, float radius = 1.f, sf::Color color = sf::Color::Red) {
		circle.setFillColor(color);
		circle.setPosition(x, y);
		circle.setRadius(radius);

		isStaticEntity = true;
	}

	inline sf::CircleShape get_circ() const { return circle; }

	inline void set_color(sf::Color color) { circle.setFillColor(color); }

	inline sf::Vector2f get_position() const { return circle.getPosition(); }

	sf::Rect<float> get_bounding_box() const { return circle.getGlobalBounds(); }

	inline void move(sf::Vector2f position) { circle.move(position); }

	inline void set_position(sf::Vector2f position) { circle.setPosition(position); }

	//sf::Vector2f get_velocity() const { return velocity; }

	//void set_velocity(sf::Vector2f velocity) { this->velocity = velocity; }

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const { target.draw(circle); }
};


#endif