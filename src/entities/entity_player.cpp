#include "entity_player.h"
#include "../game_state.h"
#include "../utils/math_functions.h"


sf::Vector2f PlayerEntity::get_player_direction() {
	sf::Vector2f direction{};
	bool joyStickFlag = sf::Joystick::isConnected(0);  // joystick slot 0

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		direction.y -= 1.f;
		joyStickFlag = false;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		direction.y += 1.f;
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		direction.x -= 1.f;
		joyStickFlag = false;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		direction.x += 1.f;
		joyStickFlag = false;
	}

	if (joyStickFlag) {
		float deadZone = 0.1f;
		sf::Vector2f stickInput(0.f, 0.f);
		stickInput.x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		stickInput.y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		float nrm = math::norm(stickInput);

		// Use a scaled radial deadzone: http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html
		if (nrm / 100 < deadZone) {
			stickInput.x = stickInput.y = 0;
		}
		else {
			stickInput = (stickInput / (nrm)) * ((nrm / 100 - deadZone) / (1 - deadZone));
		}
		direction = stickInput;
	} else {
		direction /= math::norm(direction);
	}

	return direction;
}


void PlayerEntity::plan(GameState* eng) {
	float decay = 0.5;
	velocity -= (2.f*decay - decay * decay) * velocity;

	// Player movement direction and velocity
	float speed = 170.f;
	velocity += speed * get_player_direction();
}


void PlayerEntity::act(GameState* eng, float deltaTime) {
	move(deltaTime * velocity);
}


void PlayerEntity::collide(GameState* eng, id::IdType collideeId) {
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
	velocity -= math::dot_product(velocity, normal) * normal;
}


void PlayerEntity::draw(sf::RenderTarget& target, sf::RenderStates states) {
	target.draw(circle);
}

