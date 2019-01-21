// Created by jhelland (1/20/19)
//


#ifndef __MATH_FUNCTIONS_H__
#define __MATH_FUNCTIONS_H__


#include <math.h>

#include <SFML/Graphics.hpp>


namespace math {

	inline float dot_product(const sf::Vector2f& p1, const sf::Vector2f& p2) { return p1.x*p2.x + p1.y*p2.y; }

	inline float norm(const sf::Vector2f& v) { return std::sqrt(dot_product(v, v)); }

	template<typename T>
	inline int sgn(T val) { return (T(0) < val) - (val < T(0)); }

}  // namespace math


#endif