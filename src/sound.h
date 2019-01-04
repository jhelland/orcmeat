// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  For playing repeated short sound effects.


#ifndef SOUND_H
#define SOUND_H


#include <vector>
#include <string>
#include <SFML/Audio.hpp>


namespace snd 
{
	enum soundType { blip, COUNT };

	void play_sound (soundType type, sf::Vector2f position, float volume);
}


#endif    
