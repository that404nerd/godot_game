## FSM based Movement System

- Sideways movement when falling [x]
- Add 3 points to the jump arc (Jump, Peak, Landing like a parabola) [x]
- Fix a bug where jumping during the falling state doesn't trigger the falling state after the jump [x]
- Fix a bug where jump logs are triggered twice (probably setting the state twice somewhere) [x]
- Fix Jump transition to Sprint state where friction isn't applied [x]
- Double Jump [x]

- Jump buffer []
- Infinite jump bug lol []
- Fix sudden snap to the original position when jumped during a side sprint []
- Rewrite sub-states as structs rather than enums []
- Tracking previous states (Base-states/Sub-States) []
