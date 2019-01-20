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
	sf::Vector2f get_player_direction();

	inline float dot_product(const sf::Vector2f& p1, const sf::Vector2f& p2) { return p1.x*p2.x + p1.y*p2.y; }

	inline float norm(const sf::Vector2f& v) { return std::sqrt(dot_product(v, v)); }

public:
	PlayerEntity(float x, float y, float radius = 1.f, sf::Color color = sf::Color::Red) {
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

	void plan(GameState* eng);
	void act(GameState* eng, float delta_time);
	void collide(GameState* eng, id::IdType collidee) { ; }


	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const { target.draw(circle); }
};


#endif