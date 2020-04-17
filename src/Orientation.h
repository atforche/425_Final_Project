#ifndef ORIENTATION_H
#define ORIENTATION_H

// Class that is a 2D vector that is always a unit vector
class Orientation {
private:

	double x;
	double y;

public:

	//Requires: nothing
	//Modifies: itself
	//Effects: Creates an Orientation object pointing at (0,1)
	Orientation();

	//Requires: two doubles, an x_in and a y_in
	//Modifies: itself
	//Effects: Creates an Orientation object which is a unit vector pointing at (x_in, y_in)
	Orientation(double x_in, double y_in);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns a reference to the x value of the Orientation object
	double& GetX();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns a reference to the y value of the Orientation object
	double& GetY();
};

#endif /* ORIENTATION_H */
