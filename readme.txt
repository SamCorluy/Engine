GitHub link: "https://github.com/SamCorluy/Engine"

Gameloop: the game loop consists of both an update and a static update.
the reason I left the static update in my engine is because of support reasons, that way my engine isn't specifically made to handle a single project.

ElapsedTime: both the elapsed time for the regular and static update are stored in a singleton and updated every frame.
That way I can access the elapsed time outside of my update function.

EventQueues: I use event queues for the sounds I want to play, and for assigning objects to my scenes.
The latter prevents the container of objects from growing during the scene's update(allowing this causes problems when working with an iterator)

AnimationComponent: The animation component holds multiple animation structures within itself, this is extremely useful for grouping animations that belong to a single gameObject.

MovementComponent: While not part of the engine itself, I use a movement component to determine whether an object is controlled by the player or not, when this is present, AI behavior gets ignored.

pimpl: pimpl was used for both the sound system and the inputManager