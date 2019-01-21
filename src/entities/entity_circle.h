// Created by jhelland (1/7/19)
//
// DESCRIPTION:
//	An entity for the sake of testing the engine.
// 


#ifndef ENTITY_CIRCLE_H
#define ENTITY_CIRCLE_H


#include <SFML/Graphics.hpp>


#include "../core/entity.h"
#include "../utils/math_functions.h"


class CircleEntity : public core::Entity {  
private:
	sf::CircleShape circle;

public:
	CircleEntity(float x, float y, float radius = 1.f, sf::Color color = sf::Color::Red);

	inline sf::CircleShape get_circ() const;
	inline void set_color(sf::Color color);
	inline sf::Vector2f get_position() const;
	sf::Rect<float> get_bounding_box() const;
	inline void move(sf::Vector2f dir);
	inline void set_position(sf::Vector2f position);

	void plan(GameState *eng);
	void act(GameState *eng, float deltaTime);
	void collide(GameState *eng, id::IdType collideeId);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
};

#endif