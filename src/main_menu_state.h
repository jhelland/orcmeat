// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  State manager for the main menu.
//

#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H


#include "g_states.h"
#include "state_engine.h"
#include "game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class MainMenu : public State 
{
  public:
    void initialize ();
    void clean ();

    void pause ();
    void resume ();

    void update (StateEngine*);
    void draw (StateEngine*);
    void handle_events (StateEngine*);

    static MainMenu* instance () { return &m_menu; } // allow for convenient switching between states

  // protected allows inheritance and use by friend classes
  protected:
    MainMenu () { }

  private:
    static MainMenu m_menu; // static to prevent creating new copies with every copy of the state
    sf::Texture playButton_t;
    sf::Sprite playButton;
};


#endif
