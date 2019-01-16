// Created by jhelland (1/14/19)
//


#ifndef __SPATIAL_HASH_H__
#define __SPATIAL_HASH_H__


#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>


template<typename T>
class SpatialHash {
private:
	sf::Vector2f binDims;
	std::unordered_map<sf::Vector2f, std::vector<T>> bins;

private:
	int hash(int x, int y, int bucketWidth, int bucketsPerSide) {
		return (int)(std::floor(x / bucketWidth) + floor(y / bucketWidth));
	}

public:
	SpatialHash(unsigned width, unsigned height, unsigned cellSize) {

	}
};


#endif