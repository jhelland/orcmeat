#ifndef ENTITY_CIRCLE_H
#define ENTITY_CIRCLE_H


#include <vector>

#include <SFML/Graphics.hpp>

#include "entity.h"


class CircleEntity : public Entity, public sf::Drawable {
public:
	CircleEntity(float x, float y, float radius = 1.0, sf::Color color = sf::Color::Red) {
		circle.setFillColor(color);
		circle.setPosition(x, y);
		circle.setRadius(radius);
	}
	sf::CircleShape get_circ() { return circle; }
	sf::Vector2f get_position() { return circle.getPosition(); }


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(circle);
	}

	sf::CircleShape circle;
};


#endif