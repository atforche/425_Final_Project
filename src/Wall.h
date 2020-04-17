#ifndef WALL_H
#define WALL_H
#include "Coordinate.h"
#include "ofApp.h"

// Class that does all the information needed for walls
class Wall {
private:

	Coordinate beginning;
	Coordinate ending;

public:

	//Requires: nothing
	//Modifies: nothing
	//Effects: Creates a Wall object that goes from (0,0) to (0,0)
	Wall();

	//Requires: two Coordinate objects, begin and end
	//Modifies: nothing
	//Effects: Creates a Wall object from (begin.x, begin.y) to (end.x, end.y)
	Wall(Coordinate begin, Coordinate end);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Renders the Wall object as a line from beginning to ending
	void Render();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns the Coordinate of the walls beginning
	Coordinate GetBeginning();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns the Coordinate of the walls ending
	Coordinate GetEnding();
};

#endif /* WALL_H*/