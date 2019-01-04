// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  State manager for the main game.
//


#ifndef GAME_H
#define GAME_H


#include "main_menu_state.h"
#include "pause_menu_state.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>


class GameState : public State 
{
  public:
    void initialize ();
    void clean ();

    void pause ();
    void resume ();

    void update (StateEngine*);
    void draw (StateEngine*);
    void handle_events (StateEngine*);
    //GameState();
    static GameState* instance () { return &g_state; } // allow for convenient switching between states

  // protected allows inheritance and use by friend classes	
  protected:
    GameState () { }

  private:
    static GameState g_state; // static to prevent creating new copies with every copy of the state
    std::vector<sf::CircleShape> circs;
    sf::Texture background_img;
    sf::Sprite background;
};


#endif 
