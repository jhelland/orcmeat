// Created by jratzlaff (1/20/19)
//
// DESCRIPTION:
//	Player entity
// 


#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H


#include <SFML/Graphics.hpp>

#include "../core/core.h"
#include "../utils/util_functions.h"


class PlayerEntity : public core::Entity {
private:
	sf::CircleShape circle;

private:
	sf::Vector2f get_player_direction();	

public:
	PlayerEntity(float x, float y, float radius = 1.f, sf::Color color = sf::Color::Red) {
		circle.setFillColor(color);
		circle.setPosition(x, y);
		circle.setRadius(radius);

		velocity = sf::Vector2f(0.f, 0.f);
	}

	inline sf::CircleShape get_circ() const { return circle; }

	inline void set_color(sf::Color color) { circle.setFillColor(color); }

	inline sf::Vector2f get_position() const { 
		return circle.getPosition();
	}

	sf::Rect<float> get_bounding_box() const { 
		return circle.getGlobalBounds();
	}

	inline void move(sf::Vector2f dir) { 
		circle.move(dir);
	}

	inline void set_position(sf::Vector2f position) { 
		circle.setPosition(position);
		auto box = circle.getGlobalBounds();
	}

	void plan(GameState* eng);
	void act(GameState* eng, float delta_time);
	void collide(GameState* eng, id::IdType collideeId);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
};


#endif