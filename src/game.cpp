// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Main game state. Encapsulates sound, music, and handles player input.
//


#include "game.h"
#include "sound.h"
#include "music.h"

#include <iostream>


GameState GameState::g_state; // declaration for static GameState g_state


void GameState::initialize () 
{
  // background image
	background_img.loadFromFile("textures\\background.jpg");  //"../textures/background.jpg");
	background.setTexture(background_img);

  // background music
	msc::play_music("music\\Induction.ogg");  //"../music/Induction.ogg");
}


void GameState::clean () 
{
  // clear circles
  circs.clear();

  // stop playing music
  msc::stop_music();
}


void GameState::pause () 
{
	msc::pause_music();
}


void GameState::resume () 
{
	msc::resume_music();
}

void GameState::update (StateEngine* eng) 
{
	
}

void GameState::draw (StateEngine* eng) 
{
  eng->window.setView(eng->playFieldView);
  eng->window.clear(sf::Color(0, 0, 0, 255));
	eng->window.draw(background);
	for (int i = 0; (unsigned) i < this->circs.size(); i++) {
		eng->window.draw(circs[i]);
	}
  
  // draw the board

  // test to make sure view is working properly
  eng->window.setView(eng->window.getDefaultView());
  sf::CircleShape c(30.f, 30);
  c.setFillColor(sf::Color::Magenta);
  c.setPosition(400.f, 300.f);
  eng->window.draw(c);
}

void GameState::handle_events (StateEngine *eng) 
{
	sf::Event event;
	while (eng->window.pollEvent(event)) 
  {
    std::cout << "Game\t" << event.key.code << "\t" << eng->get_stack_length() << std::endl;
		switch (event.type) 
    {
      case sf::Event::KeyPressed:
      {
        switch (event.key.code) 
        {
          case sf::Keyboard::Escape:
            pause();
            eng->push_state(PauseMenuState::instance());
            break;

          case sf::Keyboard::Space:
            break;

          case sf::Keyboard::BackSpace:
            break;

          case sf::Keyboard::A:
            break;

          default:
            break;
        }
        break;
      }

      case sf::Event::MouseButtonPressed:
      {
        switch (event.key.code) 
        {
          case sf::Mouse::Left:
          {
            sf::Vector2f v = eng->window.mapPixelToCoords(sf::Vector2i(20, 20));
            sf::CircleShape c;
            c.setRadius(20);
            c.setPosition(sf::Mouse::getPosition(eng->window).x - v.x,
                    sf::Mouse::getPosition(eng->window).y - v.y);				
            c.setFillColor(sf::Color::Red);
            circs.push_back(c);

            snd::play_sound(snd::blip, c.getPosition(), 100.f);
            break;
          }

          default:
            break;
        }
        break;
      }

      case sf::Event::LostFocus:
        pause();
        eng->push_state(PauseMenuState::instance());
        break;

      case sf::Event::Closed:
        eng->exit();
        break;

      default:
        break;
		}
	}
}
