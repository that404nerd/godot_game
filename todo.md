## FSM Movement System

- Sideways movement when falling [x]
- Add 3 points to the jump arc (Jump, Peak, Landing like a parabola) [x]
- Fix a bug where jumping during the falling state doesn't trigger the falling state after the jump [x]
- Fix a bug where jump logs are triggered twice (probably setting the state twice somewhere) [x]
- Fix Jump transition to Sprint state where friction isn't applied [x]
- Double Jump [x]

26th Nov, 2025:
- Rewrite of the entire movement system using godot signals (Everything is becoming too verbose and unstable) [] 
