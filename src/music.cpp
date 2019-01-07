// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Handles background playing of music during gameplay.


#include "music.h"


namespace msc {
	namespace {
		static sf::Music music;


		bool initialized(false);


		void init() {
			sf::Listener::setPosition(400.f, 0.f, 0.f);
			music.setLoop(false);
			music.setRelativeToListener(true);
			music.setPosition(400.f, 0.f, 300.f);
			initialized = true;
		}
	}


	void pause_music() {
		msc::music.pause();
	}


	void resume_music() {
		msc::music.play();
	}


	void play_music(std::string filename) {
		msc::music.openFromFile(filename);
		msc::music.play();
	}


	void stop_music() {
		msc::music.stop();
	}


	void update() {
		if (music.getStatus() == sf::Music::Stopped)
			play_music("music\\Induction.ogg");  //Linux, mac: "../music/Induction.ogg");
	}
}

