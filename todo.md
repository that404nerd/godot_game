## FSM based Movement System

- Sideways movement when falling [x]
- Add 3 points to the jump arc (Jump, Peak, Landing like a parabola) [x]
- Fix a bug where jumping during the falling state doesn't trigger the falling state after the jump [x]
- Fix Jump transition to Sprint state where friction isn't applied [x]

- Jump buffer []
- Fix a bug where jump logs are triggered twice (probably setting the state twice somewhere) []
- Fix sudden snap to the original position when jumped during a side sprint []
- Tracking previous states (Base-states/Sub-States) [] (FUTURE....)
