#include "Sensor.h"

Sensor::Sensor() {
	pos = Coordinate(0, 0);
	rotation = 0;
	orientation.GetX() = cos(rotation * (3.14 / 180.0));
	orientation.GetY() = sin(rotation * (3.14 / 180.0));
	walls = std::vector<Wall>();
}

//--------------------------------------------------------------------------------------

Sensor::Sensor(double x_in, double y_in, double rotation_in) {
	pos = Coordinate(x_in, y_in);
	rotation = rotation_in;
	orientation.GetX() = cos(rotation * (3.14 / 180.0));
	orientation.GetY() = sin(rotation * (3.14 / 180.0));
	walls = std::vector<Wall>();
}

//--------------------------------------------------------------------------------------

void Sensor::AddWalls(std::vector<Wall>& walls_in) {
	walls = walls_in;
}

//--------------------------------------------------------------------------------------

void Sensor::Render() {
	ofSetColor(255, 0, 0);
	ofSetLineWidth(1);
	Coordinate wall_collision = Wall_Collision(Calculate_Distance());
	ofDrawLine(pos.GetX(), pos.GetY(), wall_collision.GetX(), wall_collision.GetY());
}

//--------------------------------------------------------------------------------------

double Sensor::Calculate_Distance() {
	double min_dist = DBL_MAX;

	for (Wall wall : walls) {
		//First modify the coordinates and orientations to make sense in an XY place
		Coordinate ray_origin = Coordinate(pos.GetX(), -pos.GetY());
		Orientation ray_orientation = Orientation(orientation.GetX(), -orientation.GetY());
		Coordinate segment_a = Coordinate(wall.GetBeginning().GetX(), -wall.GetBeginning().GetY());
		Coordinate segment_b = Coordinate(wall.GetEnding().GetX(), -wall.GetEnding().GetY());

		//Now calculate the distance https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
		Coordinate v1 = Subtract(ray_origin, segment_a);
		Coordinate v2 = Subtract(segment_b, segment_a);
		Coordinate v3 = Coordinate(-ray_orientation.GetY(), ray_orientation.GetX());

		double t1 = Cross_Product(v2, v1) / Dot_Product(v2, v3);
		double t2 = Dot_Product(v1, v3) / Dot_Product(v2, v3);

		if (t1 >= 0 && (0 <= t2 && t2 <= 1)) {
			if (t1 < min_dist) {
				min_dist = t1;
			}
		}
	}
	return min_dist;
}

//--------------------------------------------------------------------------------------

double Sensor::Camera_Calculate_Distance(ofColor& color_in) {
	double min_dist = DBL_MAX;
	int min_index = -1;

	for (size_t i = 0; i < walls.size(); ++i) {
		//First modify the coordinates and orientations to make sense in an XY place
		Coordinate ray_origin = Coordinate(pos.GetX(), -pos.GetY());
		Orientation ray_orientation = Orientation(orientation.GetX(), -orientation.GetY());
		Coordinate segment_a = Coordinate(walls[i].GetBeginning().GetX(), -walls[i].GetBeginning().GetY());
		Coordinate segment_b = Coordinate(walls[i].GetEnding().GetX(), -walls[i].GetEnding().GetY());

		//Now calculate the distance https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
		Coordinate v1 = Subtract(ray_origin, segment_a);
		Coordinate v2 = Subtract(segment_b, segment_a);
		Coordinate v3 = Coordinate(-ray_orientation.GetY(), ray_orientation.GetX());

		double t1 = Cross_Product(v2, v1) / Dot_Product(v2, v3);
		double t2 = Dot_Product(v1, v3) / Dot_Product(v2, v3);

		if (t1 >= 0 && (0 <= t2 && t2 <= 1)) {
			if (t1 < min_dist) {
				min_dist = t1;
				min_index = i;
			}
		}
	}
	color_in = walls[min_index].Get_Color();
	return min_dist;
}

//--------------------------------------------------------------------------------------

Coordinate Sensor::Wall_Collision(double min_dist) {
	double new_x = pos.GetX() + orientation.GetX() * min_dist;
	double new_y = pos.GetY() + orientation.GetY() * min_dist;
	return Coordinate(new_x, new_y);
}


//--------------------------------------------------------------------------------------

double& Sensor::GetX() {
	return pos.GetX();
}

//--------------------------------------------------------------------------------------

double& Sensor::GetY() {
	return pos.GetY();
}

//--------------------------------------------------------------------------------------

double& Sensor::GetRotation() {
	return rotation;
}

//--------------------------------------------------------------------------------------

Orientation& Sensor::GetOrientation() {
	return orientation;
}

void Sensor::Rotate(double rotation_in) {
	rotation += rotation_in;
	rotation = fmod(rotation, 360);
	orientation.GetX() = cos(rotation * (PI / 180.0));
	orientation.GetY() = sin(rotation * (PI / 180.0));
}