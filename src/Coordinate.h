#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>

// Class that holds all the information needed for Coordinates
class Coordinate {
private:

	double x;
	double y;

public:

	//Requires: nothing
	//Modifies: nothing
	//Effects: Creates a Coordinate object at (0,0)
	Coordinate();

	//Requires: a double x and a double y
	//Modifies: nothing
	//Effects: Creates a Coordinate object at (x,y)
	Coordinate(double x_in, double y_in);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns a reference to the Coordinate's x value
	double& GetX();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns a reference to the Coordinate's y value
	double& GetY();

};

//Requires: two Coordinates
//Modifies: nothing
//Effects: Returns the cross product of the two coordinates
double Cross_Product(Coordinate one, Coordinate two);

//Requires: two Coordinates
//Modifies: nothing
//Effects: Returns the dot product of the two coordinates
double Dot_Product(Coordinate one, Coordinate two);

//Requires: two Coordinates
//Modifies: nothing
//Effects: Returns the distance between the two coordinates
double Distance(Coordinate one, Coordinate two);

//Requires: two Coordinates
//Modifies: nothing
//Effects: Returns a coordinate equal to one minus two
Coordinate Subtract(Coordinate one, Coordinate two);

#endif 
