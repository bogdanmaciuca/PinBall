# PinBall in OpenGL

## [Main Loop](src/main_loop.cpp)

The loop for the game.  

### [MainLoop()](src/main_loop.cpp#L3-L36)

Runs through the different update funcitons with a fixed timestep. 
The update() function returns true when the game needs to reset.  

## [Main](src/main.cpp)

The program needs to start somewhere.  

### [main()](src/main.cpp#L42-L71)

 - creates the world
 - runs the game loop

## [Object](src/object.cpp)

## [World](src/world.cpp)

The world that defines the rules that we work with in the game. It initializes the different models and updates their rendering and movement.  

### [World()](src/world.cpp#L12-L107)

Initializes the Marble, the Edge, the Drain, the Two Paddles and any obstacles we desire.

### [UpdateModels()](src/world.cpp#L109-L113)

Calls the [UpdateModel()](#) function for every model.

### [UpdatePads()](src/world.cpp#L115-135)

Detects keyboard presses and updates the rotation ot the paddles acodingly.  
The move_towards() lambda funciton updates the padle's rotation and angular velocity.  

### [Render()](src/world.cpp#L137-L142)

Draws each model.  

## [Physics](src/physics.cpp)

In physics.cpp is where the ball's collisions and movement is calculated.

### [UpdatePhysics()](src/physics.cpp#L111-L127)

The only function meant to be interacted with through the means of the [world class](src/world.hpp). It does the following actions in order:  
 - Resets the normal, surface_velocity, and collision_count variables. 
 - Checks to see if the marble touched the drain and the game needs to be reset.
 - Checks and calculates the collisions for every other object.
 - Integrates the marbles new position.

### [CheckCollision()](src/physics.cpp#L72-L80)

The first function to get called inside [UpdatePhysics()](#updatephysics). It splits the collisions in two types:

 - [Ball to Ball collisions](#checkballballcollision)
 - [Ball to Polygon collisions](#)

### [CheckBallBallCollision()](src/physics.cpp#L59-L70)

Compares the distance between the two with the sum of the radiouses, if the distance is smaller then it makes sure the marble dosn't collide into the ball and then [resolves the collision](#resolvecollision).

### [CheckBallPolygonCollision()](src/physics.cpp#L35-L57)

Calculates the [closest point on segment](#closestPointOnSegment) for the marble to each edge, then compares the distance between the marble's center and the point with the marble's radious. If it is smaller it makes sure the marble doesn't collide into the ball and then [resolves the collision](#resolvecollision). It also treats the case where the ball impacts on the corner with the nest edge and so ignores it.

### [ResolveCollision()](src/physics.cpp#L19-L33)

Takes the normal vector from the ball impacting the terrain and adds it to a normal pool, this is done so that the order in which impacts get resolved does not affect the end result. We only get one normal that represents the cumulation of normals. Not only that, but if the ball registers extra collisions with extra edges it will minimize their impact.  

After that we check the surface velocity compared to the marble. As we only have rotation for any object that isn't the marble we only need to calculate velocity for the points rotation. We calculate the vector from the object's position to the collision point and then resolve the appropriate matrix for the rotation to get the velocity vector.

### [closestPointOnSegment()](src/physics.cpp#L7-L12)

Since my search gave me conflicting information on if glm::closestPointOnLine() clamps the result to the serment line or not, and my intuition said probably not, this function was implemented in order to resolve the issue.  
it calculates the following formula $\frac{(p-a) \cdot (b-a)}{||b-a||^2}$ in order to find the scalar with which we can mutiply $\vec{ab}$ to get the path from a to p. We clamp this result between 0 and 1 in order to have it remain on the segment line.

### [Integrate()](src/physics.cpp#L82-L108)

 - Calculates the redirected force by:
   - Normalizeing the normal 
   - Calculating the relative surface velocity
   - Subtract the normal multiplied by the average surface velocity and the bounce power
 - Apply drag.  
 - Apply gravity.
 - Modify the position.
