
# Engine Architecture

hashmap<uuid, entities> ents
vector<ent*> dist0, dist1, dist2
render window
sound: effects, music
camera

game loop
background loop


# Game loop
get input
queue player actions
for each entity in dist0, dist1:
  queue entity actions
perform queued actions
check collisions:
  queue collisions, do corresp. actions
render

# Background loop
distancer category updates: promote entity to closer dist
achievements (?)
dist2 actions

# Initial demo specs
- Encampment setting: enclosing wall
- small enemy, larger enemy, one power, meat
- player, basic combat, movement
- building
- world generator: randomly place everything
