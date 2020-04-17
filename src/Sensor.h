#ifndef SENSOR_H
#define SENSOR_H
#include "Coordinate.h"
#include "Orientation.h"
#include "Wall.h"
#include <vector>

class Sensor {
private:

	Coordinate pos;
	Orientation orientation; //Indicates orientation of sensor as unit vector
	double rotation; //Indicates orientation of sensor as degrees
	std::vector<Wall> walls;

public:

	//Requires: nothing
	//Modifies: itself
	//Effects: Creates a default sensor object at (0,0)
	Sensor();

	//Requires: Coordinate pos_in, double rotation_in
	//Modifies: itself
	//Effects: Creates a sensor object at (pos.x, pos.y) pointing toward rotation_in
	Sensor(double x_in, double y_in, double rotation_in);

	//Requires: a vector of wall objects
	//Modifies: itself
	//Effects: sets the sensors wall vector equal to the input
	void AddWalls(std::vector<Wall>& walls_in);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Renders the sensor object as a red line from pos to its collision with a wall
	void Render();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Calculates the distance from the sensor to the nearest wall
	double Calculate_Distance();

	//Requires: min_dist to a wall
	//Modifies: nothing
	//Effects: Returns the point of intersection between the sensor and the nearest wall
	Coordinate Wall_Collision(double min_dist);

	//Requires: nothing
	//Modifies: itself
	//Effects: Returns a reference to the x value of the sensor
	double& GetX();

	//Requires: nothing
	//Modifies: itself
	//Effects: Returns a reference to the y value of the sensor
	double& GetY();

	//Requires: nothing
	//Modifies: itself
	//Effects: Returns a reference to the rotation value of the sensor
	double& GetRotation();

	//Requires: nothing
	//Modifies: itself
	//Effects: Returns a referece to the orientation value of the sensor
	Orientation& GetOrientation();
	

};

#endif /* SENSOR_H */
