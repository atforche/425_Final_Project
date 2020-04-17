#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include "Wall.h"
#include "Robot.h"

// Class that holds all of the information for the overall simulation
class Sim {
private:

	std::vector<Wall> walls;
	std::vector<Robot*> robots;
	bool halted;
	double window_width;
	double window_height;

public:

	//Requires: no input
	//Modifies: nothing
	//Effects: Creates a new simulation object with no walls and no robots with default size 800 x 800
	Sim();

	//Requires: two double, the window width and the window height
	//Modifies: nothing
	//Effects: Creates a new simulation object with no walls and no robots with size window_width x window_height
	Sim(double window_width_in, double window_height_in);

	//Requires: a robot object
	//Modifies: nothing
	//Effects: Adds the robot object to the simulations list of robots
	void Add_Robot(Robot& robot);

	//Requires: a wall object
	//Modifies: nothing
	//Effects: Adds the wall object to the simulations list of walls
	void Add_Wall(Wall& wall);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Renders all of the objects currently associated with the current simulation
	void Render(bool sensors = false);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns whether or not the simulation has halted
	bool IsHalted();

	//Requires: two doubles, the window width and the window height
	//Modifies: itself
	//Effects: Creates four walls around the edge of the area
	void Initialize();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns a vector of the walls in the simulation
	std::vector<Wall> GetWalls();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns a vector of the robots in the simulation
	std::vector<Robot*> GetRobots();

	//Requires: nothing
	//Modifies: itself
	//Effects: Clears all of the objects from the simulation
	void Reset();

	~Sim();

};

#endif /* Simulation_H*/