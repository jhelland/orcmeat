// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Archetype engine manages states and rendering: inherited by e.g. MainMenuState
//
// TODO: 
//  (jhelland) Virtualize to allow multiple engines on one window?
//  (jhelland) Allow engine to handle multiple states at once?


#ifndef STATE_ENGINE_H
#define STATE_ENGINE_H


#include <stack>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class State; // Forward declare; defined in src/g_states 


class StateEngine 
{
  public:
    StateEngine (std::string title);
    ~StateEngine ();

    void change_state (State*);
    void push_state (State*);
    void pop_state ();
    void clear_states ();

    void handle_events ();
    void update ();
    void draw ();

    bool is_running () { return bRunning; }
    void exit () { bRunning = false; }

    int get_stack_length () { return states.size(); }

    sf::RenderWindow window;
    sf::View playFieldView;

  private:
    std::stack<State*> states;

    bool bRunning;
    const int nWinWidth = 440;
    const int nWinHeight = 600;
};


#endif
