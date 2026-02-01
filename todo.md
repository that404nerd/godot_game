## FSM Movement System

- Sideways movement when falling [x]
- Add 3 points to the jump arc (Jump, Peak, Landing like a parabola) [x]
- Fix a bug where jumping during the falling state doesn't trigger the falling state after the jump [x]
- Fix a bug where jump logs are triggered twice (probably setting the state twice somewhere) [x]
- Fix Jump transition to Sprint state where friction isn't applied [x]
- Double Jump [x]
- Rewrite of the entire movement system using godot signals (Everything is becoming too verbose and unstable) [x] 
- Fix spamming the crouch key during slide state (causes the collision shape to glitch into the floor) [x]
- Fix un-crouching when there's an object above the player [x]
- Have a function for setting state strings in emit_signal() to avoid naming issues [x]
- Singletons for accessing player and state machine instances [x]

## Issues
- Transition to idle state when sliding into a world objects []
 Allow sliding when toggled during falling state []
- ObjectDB and resource leak fix []
- Animations for different states []
