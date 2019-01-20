#include "entity_player.h"
#include "../game_state.h"

sf::Vector2f PlayerEntity::get_player_direction() {
	sf::Vector2f direction{};
	bool joyStickFlag = sf::Joystick::isConnected(0);  // joystick slot 0

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		direction.y -= 1.f;
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		direction.x -= 1.f;
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		direction.y += 1.f;
		joyStickFlag = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		direction.x += 1.f;
		joyStickFlag = false;
	}

	if (joyStickFlag) {
		float deadZone = 0.1f;
		sf::Vector2f stickInput(0.f, 0.f);
		stickInput.x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		stickInput.y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		float nrm = norm(stickInput);

		// Use a scaled radial deadzone: http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html
		if (nrm / 100 < deadZone) {
			stickInput.x = stickInput.y = 0;
		}
		else {
			stickInput = (stickInput / (nrm)) * ((nrm / 100 - deadZone) / (1 - deadZone));
		}
		direction = stickInput;
	}

	return direction;
}

void PlayerEntity::plan(GameState* eng) {
	float decay = 0.5;
	velocity -= (2.f*decay - decay * decay) * velocity;

	// Player movement
	float speed = 170.f;
	velocity += speed * get_player_direction();
}

void PlayerEntity::act(GameState *eng, float delta_time) {
	position += velocity*delta_time;
	circle.setPosition(position);
}
