- Raycasts when you need the enemy to detect the player and see if it collides
  and switch to combat state and shoot and handle it. <br/>
  The fov is important as it determines how much the enemy can see.  <br/>
  The vision component uses the fov for the player visibility. The Vision Component is
  directly attached to the character's head (from the skeleton).

  It would be better if we limit the vision checking i.e raycasts collision
  checks to the detection area...

- Detection area to let the enemy know the player is near. Could be used for
  other components say AudibleComponent or something like that.

- How can we use the detection area, the raycasts, the fov all
  together, distance b/w player and enemy?

  a) So for the detection area, this triggers the **SEARCH STATE** for the enemy (say 50m). <br/>
  And then you might have an arbitrary range of say 30m where the enemy could go into the **COMBAT STATE** <br/>
  b) The FOV and Raycasts will be used for the following:
     - The FOV will define how wide the enemy can see like a cone shape. And the Raycasts can be used for
     line of sight which allows the enemy to detect whether the raycast collided with the Player or some obstruction.
    - For example:
      - 50m area: keeps track of potential entities.
      - 30m: maximum distance at which vision can trigger.
      - 120 deg: angular restriction.
      - Raycast: confirms there's no wall/obstacle between eye and target.
      - 10m: attack/combat range.