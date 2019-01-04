// Created by jhelland (12/21/18)
// 
// DESCRIPTION:
//  State manager for the pause menu.
//


#ifndef PAUSE_MENU_STATE_H
#define PAUSE_MENU_STATE_H


#include "g_states.h"
#include "state_engine.h"
#include "main_menu_state.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class PauseMenuState : public State 
{
  public:
    void initialize ();
    void clean ();

    void pause ();
    void resume ();

    void update (StateEngine*);
    void draw (StateEngine*);
    void handle_events (StateEngine*);

    static PauseMenuState* instance () { return &state; }

  protected:
    PauseMenuState () { }

  private:
    static PauseMenuState state;

    sf::CircleShape p_circ;
};


#endif
