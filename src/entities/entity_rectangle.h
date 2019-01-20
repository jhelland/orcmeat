// Created by jhelland (1/18/19)
//


#ifndef __ENTITY_RECTANGLE_H__
#define __ENTITY_RECTANGLE_H__


#include <SFML/Graphics.hpp>

#include "../core/entity.h"


class RectangleEntity : public core::Entity {
private:
	sf::RectangleShape rectangle;

public:
	RectangleEntity(float x1, float y1, float x2, float y2, sf::Color color = sf::Color::Red) {
		float width = abs(x1 - x2);
		float height = abs(y1 - y2);
		rectangle.setPosition(x1, y1);
		rectangle.setSize(sf::Vector2f(width, height));
		rectangle.setFillColor(color);

		velocity = sf::Vector2f(0.f, 0.f);

		isStaticEntity = true;
	}

	inline sf::Vector2f get_position() const { return rectangle.getPosition(); }

	inline sf::Rect<float> get_bounding_box() const { return rectangle.getGlobalBounds(); }

	inline void move(sf::Vector2f position) { rectangle.move(position); }

	inline void set_position(sf::Vector2f position) { rectangle.setPosition(position); }

	void plan(GameState* eng) {}
	void act(GameState* eng, float delta_time) {}
	void collide(GameState* eng, id::IdType collidee) {}
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const { target.draw(rectangle); }
};


#endif