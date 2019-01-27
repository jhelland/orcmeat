// Created by jhelland (1/26/19)
//


#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include <vector>
#include <array>

#include <SFML/Graphics.hpp>


namespace core {

	class Animation {
	public:
		int frameCount;
		int frameInterval;
		bool looping;
		int spriteIdx;
		int animationIdx;
		int numAnimations;

	public:
		std::vector<std::array<int, 5>> animationSpecs;
		std::vector<std::vector<int>> sequences;

	public:
		Animation();
		Animation(const int frameInterval, const bool looping = false);

		void register_animation(const int x, const int y, const int width, const int height, const int gap, std::vector<int> sequence);
		void update(sf::Sprite& sprite);
		void reset();
		void change_animation(const int idx, const int frameInterval);
	};  // class Animation

}  // namepsace core


#endif