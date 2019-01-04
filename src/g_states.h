// Created by jhelland (12/21/18)
//
// DESCRIPTION:
//  Base class for future states.
//


#ifndef G_STATES_H
#define G_STATES_H


#include "state_engine.h"


class State 
{
  public:
    virtual void initialize () = 0;
    virtual void clean () = 0;
    
    virtual void pause () = 0;
    virtual void resume () = 0;
    
    virtual void update (StateEngine*) = 0;
    virtual void draw (StateEngine*) = 0;
    virtual void handle_events (StateEngine*) = 0;

    void change_state (StateEngine* eng, State* st) { eng->change_state(st); }

  // protected allows inheritance and use by friend classes
  protected:
    State () { }
};

#endif
