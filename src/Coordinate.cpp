#include "Coordinate.h"

Coordinate::Coordinate() {
	x = 0;
	y = 0;
}

//--------------------------------------------------------------------------------------

Coordinate::Coordinate(double x_in, double y_in) {
	x = x_in;
	y = y_in;
}

//--------------------------------------------------------------------------------------

double& Coordinate::GetX() {
	return x;
}

//--------------------------------------------------------------------------------------

double& Coordinate::GetY() {
	return y;
}

//--------------------------------------------------------------------------------------

double Cross_Product(Coordinate one, Coordinate two) {
	return (one.GetX() * two.GetY()) - (one.GetY() * two.GetX());
}

//--------------------------------------------------------------------------------------

double Dot_Product(Coordinate one, Coordinate two) {
	return (one.GetX() * two.GetX()) + (one.GetY() * two.GetY());
}

//--------------------------------------------------------------------------------------

double Distance(Coordinate one, Coordinate two) {
	double a = one.GetX() - two.GetX();
	double b = one.GetY() - two.GetY();
	return std::sqrt((a * a) + (b * b));
}

//--------------------------------------------------------------------------------------

Coordinate Subtract(Coordinate one, Coordinate two) {
	return Coordinate(one.GetX() - two.GetX(), one.GetY() - two.GetY());
}
