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
#include "../core/animation.h"


class PlayerEntity : public core::Entity {
private:
	sf::FloatRect boundingBox;
	sf::Texture texture;
	sf::Sprite sprite;
	core::Animation animation;

private:
	sf::Vector2f get_player_direction();	

public:
	PlayerEntity(float x, float y, float radius = 1.f, sf::Color color = sf::Color::Red);

	inline sf::Vector2f get_position() const;
	sf::FloatRect get_bounding_box() const;
	inline void move(sf::Vector2f dir);
	inline void set_position(sf::Vector2f position);

	void plan(GameState* eng);
	void act(GameState* eng, float delta_time);
	void collide(GameState* eng, id::IdType collideeId);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
};


#endif