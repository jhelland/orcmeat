// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  For playing short repeated sound effects.
//


#include <iostream>

#include "sound.h"


namespace snd {
	namespace {
		static std::vector<sf::SoundBuffer> sounds(snd::COUNT);
		static std::vector<sf::Sound> soundChannel(64);
		static std::vector<bool> loaded(snd::COUNT);
		static bool initialized(false);


		void load_sound(soundType type, std::string filename) {
			if (!sounds[type].loadFromFile(filename)) {
				std::cout << "ERROR: unable to load sound file -- " << filename << std::endl;
				return;
			}
			loaded[type] = true;
		}


		void init() {
			for (int i = 0; i < COUNT; i++)
				loaded[i] = false;
			//sf::Listener::setPosition(400.f, 0.f, 0.f);
			initialized = true;
		}
	}


	void play_sound(soundType type, sf::Vector2f position, float volume) {
		if (!initialized)
			init();

		if (loaded[type]) {
			soundChannel[type].setBuffer(sounds[type]);
			//soundChannel[type].setPosition(position.x, 0.f, 0.f);
			soundChannel[type].setVolume(volume);
			soundChannel[type].play();
		}
		else {
			switch (type) {
			case blip:
				load_sound(type, "sounds\\blip.ogg");  // Linux, mac: "../sounds/blip.wav");
				break;

			case COUNT:
				std::cout << "ERROR: COUNT is not a valid sound type\n";
				break;
			}

			if (loaded[type])
				play_sound(type, position, volume);
		}
	}
}
