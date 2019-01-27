// Created by jhelland (1/26/19)
//


#include <assert.h>

#include "animation.h"


namespace core {

	Animation::Animation() : frameCount(0), spriteIdx(0), animationIdx(0) {}


	Animation::Animation(const int frameInterval, const bool looping) : 
		frameInterval(frameInterval), looping(looping) {
		frameCount = 0;
		spriteIdx = 0;
	}


	void Animation::register_animation(const int x, const int y, const int width, const int height, const int gap, std::vector<int> sequence) {
		animationSpecs.push_back(std::array<int, 5>{ x, y, width, height, gap });
		sequences.push_back(sequence);
	}


	void Animation::update(sf::Sprite& sprite) {
		if (!looping && spriteIdx + 1 == sequences.size()) {
			return;
		}

		// Change animations every fixed number of frames
		if (frameCount + 1 == frameInterval) {
			spriteIdx = (spriteIdx + 1) % sequences.size();

			int idx = sequences[animationIdx][spriteIdx];
			auto dims = animationSpecs[animationIdx];
			sf::IntRect rect{ dims[0] + spriteIdx * dims[4], dims[1], dims[2], dims[3] };
			sprite.setTextureRect(rect);

			frameCount = 0;
		}
		frameCount = (frameCount + 1) % frameInterval;
	}


	void Animation::reset() {
		frameCount = 0;
		spriteIdx = 0;
	}


	void Animation::change_animation(const int idx, const int frameInterval) {
		assert(idx < sequences.size() && "Animation index out of range");

		if (idx != animationIdx) {
			animationIdx = idx;
			frameCount = frameInterval - 1;  // immediately start next animation frame
			this->frameInterval = frameInterval;
		}
	}
}  // namespace core