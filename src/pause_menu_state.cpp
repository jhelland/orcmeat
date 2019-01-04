// Created by jhelland (12/21/18)
// 
// DESCRIPTION:
//  State manager for the pause menu.
//


#include "pause_menu_state.h"

#include <iostream>


PauseMenuState PauseMenuState::state;


void PauseMenuState::initialize () 
{
	this->p_circ.setRadius(30);
	this->p_circ.setFillColor(sf::Color::Green);
}


void PauseMenuState::clean () 
{
	
}


void PauseMenuState::pause () 
{
	
}


void PauseMenuState::resume () 
{
	
}


void PauseMenuState::update (StateEngine* eng) 
{
    
}


void PauseMenuState::draw (StateEngine* eng) 
{
  eng->window.setView(eng->window.getDefaultView());
	eng->window.draw(this->p_circ);
}


void PauseMenuState::handle_events (StateEngine* eng) 
{
	sf::Event event;
	while (eng->window.pollEvent(event)) 
  {
    std::cout << "PauseMenu\t" << event.key.code << "\t" << eng->get_stack_length() << std::endl;
		switch (event.type) 
    {
      case sf::Event::KeyPressed:
      {
        switch (event.key.code)
        {
          case sf::Keyboard::Space:
            eng->pop_state();
            break;

          case sf::Keyboard::Escape:
            eng->clear_states();
            eng->push_state(MainMenu::instance());
            break;

          default:
            break;
        }
      }

      default:
        break;
    }
  }
}
