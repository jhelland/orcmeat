// Create by jhelland (12/21/18)
//
// DESCRIPTION:
//  State manager for the main menu
//


#include "main_menu_state.h"

#include <iostream>


MainMenu MainMenu::m_menu;


void MainMenu::initialize () 
{
	playButton_t.loadFromFile("textures\\flames4.jpg"); //"../textures/flames4.jpg");
	playButton.setTexture(playButton_t);
}


void MainMenu::clean () 
{
	
}


void MainMenu::pause () 
{
	
}


void MainMenu::resume () 
{
	
}


void MainMenu::update (StateEngine* eng) 
{
	
}


void MainMenu::draw (StateEngine* eng) 
{
  eng->window.setView(eng->window.getDefaultView());
	eng->window.draw(playButton);
}


void MainMenu::handle_events (StateEngine* eng) 
{
	sf::Event event;
	while (eng->window.pollEvent(event)) 
  {
    std::cout << "MainMenu\t" << event.key.code << "\t" << eng->get_stack_length() << std::endl;
		switch (event.type) 
    {
      case sf::Event::KeyPressed:
      {
        switch (event.key.code) 
        {
          case sf::Keyboard::Escape:
          case sf::Keyboard::Q:
            eng->exit(); // since this is the main menu, we should quit here
            break;

          case sf::Keyboard::Space:
            eng->change_state(GameState::instance());
            // TODO: add more menu options here
          
          default:
            break;
        }
        break;
      }

      case sf::Event::Closed:
        eng->exit();
        break;

      default:
        break;
		}
	}
}
