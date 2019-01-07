# DEPENDENCIES
- SFML 2.5.1: https://www.sfml-dev.org/download/sfml/2.5.1/ 
- stduuid: https://github.com/mariusbancila/stduuid


# Engine Architecture
------
### Game state
hashmap<uuid, entities> ents<br/>
vector<ent*> dist0, dist1, dist2<br/>
render window<br/>
sound: effects, music<br/>
camera<br/>

game loop<br/>
background loop

### Entities
hashmap<uuid, purgeFlag> components<br/>
if purgeFLAG
  remove component
  
### Components
uuid
functions

### Systems
Look for entities with associated component(s) and perform logic

# Game loop
------
get input<br/>
queue player actions<br/>
for each entity in dist0, dist1:<br/>
  queue entity actions<br/>
perform queued actions<br/>
check collisions:<br/>
  queue collisions, do corresp. actions<br/>
render

# Background loop
------
distancer category updates: promote entity to closer dist<br/>
achievements (?)<br/>
dist2 actions

# Initial demo specs
------
- Encampment setting: enclosing wall
- small enemy, larger enemy, one power, meat
- player, basic combat, movement
- building
- world generator: randomly place everything
