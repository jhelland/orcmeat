// Created by jhelland (1/21/19)
//


#include "entity_circle.h"
#include "../game_state.h"


CircleEntity::CircleEntity(float x, float y, float radius, sf::Color color) {
	circle.setFillColor(color);
	circle.setPosition(x, y);
	circle.setRadius(radius);
}


inline sf::CircleShape CircleEntity::get_circ() const { return circle; }


inline void CircleEntity::set_color(sf::Color color) { circle.setFillColor(color); }


inline sf::Vector2f CircleEntity::get_position() const { return circle.getPosition(); }


sf::Rect<float> CircleEntity::get_bounding_box() const { return circle.getGlobalBounds(); }


inline void CircleEntity::move(sf::Vector2f dir) {
	circle.move(dir);
}


inline void CircleEntity::set_position(sf::Vector2f position) {
	circle.setPosition(position);
}


void CircleEntity::plan(GameState *eng) {}


void CircleEntity::act(GameState *eng, float deltaTime) {
	move(deltaTime * velocity);
}


void CircleEntity::collide(GameState *eng, id::IdType collideeId) {
	auto box = get_bounding_box();
	auto p1 = get_position();
	p1.x += box.width / 2.f;
	p1.y += box.height / 2.f;

	auto collidee = eng->entityManager.get_entity_by_id(collideeId);
	auto collBox = collidee->get_bounding_box();
	auto p2 = collidee->get_position();
	p2.x += collBox.width / 2.f;
	p2.y += collBox.height / 2.f;

	auto diff = p1 - p2;

	sf::Vector2f normal(0.f, 0.f);
	float dist = 0.f;
	float penetration = 0.f;
	if (std::abs(diff.x) > std::abs(diff.y)) {
		normal.x = (float)math::sgn(diff.x);
		dist = std::abs(diff.x);
		penetration = std::abs(box.width / 2.f + collBox.width / 2.f - dist);
	}
	else {
		normal.y = (float)math::sgn(diff.y);
		dist = std::abs(diff.y);
		penetration = std::abs(box.height / 2.f + collBox.height / 2.f - dist);
	}

	move(0.5f * penetration * normal);
	velocity *= -1.f;
	//velocity -= math::dot_product(velocity, normal) * normal;
}


void CircleEntity::draw(sf::RenderTarget& target, sf::RenderStates states) {
	target.draw(circle);
}
