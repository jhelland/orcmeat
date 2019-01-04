// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  For playing longer sound files i.e. background music.
//


#ifndef MUSIC_H
#define MUSIC_H


#include <SFML/Audio.hpp>
#include <string>


namespace msc 
{
	void play_music (std::string filename);

	void pause_music ();

	void resume_music ();

  void stop_music ();

	void update ();
}


#endif
