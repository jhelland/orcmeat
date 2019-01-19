// Created by jhelland (1/18/19)
//


#ifndef __ENTITY_CAMERA_H__
#define __ENTITY_CAMERA_H__


#include <SFML/Graphics.hpp>

#include "../core/entity.h"
#include "../utils/util_functions.h"


class Camera {
private:
	sf::Vector2f position;
	sf::Clock clock;

public:
	sf::Vector2f velocity;
	bool screenShakeFlag;

public:
	Camera() : screenShakeFlag(false) {}

	inline sf::Vector2f get_position() const { return position; }
	
	void set_position(sf::Vector2f position) { this->position = position; }

	void lerp_position(const sf::Vector2f& position, const float& t = 0.038f) { this->position += (1-(1-t)*(1-t)) * (position - this->position); }
	
	void move(sf::Vector2f position) { this->position += position; }

	void start_screen_shake() { 
		if (!screenShakeFlag) {
			screenShakeFlag = true;
			clock.restart();
		}
	}

	void update(sf::View& view) {
		if (screenShakeFlag && clock.getElapsedTime().asSeconds() < 0.1f) {
			view.setRotation(utils::rand());
			view.move(5.f * sf::Vector2f(utils::rand(), utils::rand()));
		} else {
			view.setRotation(0.f);
			screenShakeFlag = false;
		}
	}
};


#endif