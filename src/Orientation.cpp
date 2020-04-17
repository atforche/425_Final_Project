#include "Orientation.h"
#include <cmath>

Orientation::Orientation() {
	x = 0;
	y = 1;
}

//--------------------------------------------------------------------------------------

Orientation::Orientation(double x_in, double y_in) {
	double magnitude = std::sqrt((x_in * x_in) + (y_in * y_in));
	x = x_in / magnitude;
	y = y_in / magnitude;
}

//--------------------------------------------------------------------------------------

double& Orientation::GetX() {
	return x;
}

//--------------------------------------------------------------------------------------

double& Orientation::GetY() {
	return y;
}