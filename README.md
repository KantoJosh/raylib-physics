# TODO

## Resources

- https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/ for collision detection, response, linear/angular motion,etc.

## Dynamics

- Add net force to Body (see https://blog.winter.dev/2020/designing-a-physics-engine/ for kinematics equations)
- Change inverted mass to mass. To represent infinite masses, just add an addiitonal attribute isInfiniteMass instead
- Run physics update loop in its own thread
- Use lookup table for different object materials to set
- Explicit Euler is being used for calculating velocities. See https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/2linearmotion/Physics%20-%20Linear%20Motion.pdf to use another more precise method (e.g. Semi - Implicit Euler)