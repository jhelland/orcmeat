// Created by jhelland (1/18/19)
//


#ifndef __ENTITY_CAMERA_H__
#define __ENTITY_CAMERA_H__


#include <SFML/Graphics.hpp>

#include "../core/entity.h"
#include "../utils/util_functions.h"
#include "../utils/math_functions.h"


class Camera {
private:
	sf::Vector2f position;
	sf::Clock clock;
	float radiusSquared;

public:
	sf::Vector2f velocity;
	bool screenShakeFlag;

public:
	Camera() : screenShakeFlag(false), radiusSquared(0.f) {}

	inline sf::Vector2f get_position() const { return position; }
	
	void set_position(sf::Vector2f position) { 
		auto diff = this->position - position;
		float dist = std::abs(math::dot_product(diff, diff));
		if (dist >= radiusSquared) {
			this->position += position - this->position;
		}
	}

	void lerp_position(const sf::Vector2f& position, const float& t = 0.038f) { 
		this->position += (1 - (1 - t)*(1 - t)) * (position - this->position);
	}
	
	void move(const sf::Vector2f& dir) { 
		float dist = std::abs(math::dot_product(dir, dir));
		if (dist >= radiusSquared) {
			this->position += dir;
		}
	}

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