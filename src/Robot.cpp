#include "Robot.h"
#define _USE_MATH_DEFINES

Robot::Robot() {
	walls = std::vector<Wall>();
	pos = Coordinate(0, 0);
	orientation = Orientation(0, 1);
	rotation = 0;
	speed = 15;
	walls = std::vector<Wall>();
}

//--------------------------------------------------------------------------------------

Robot::Robot(double x_in, double y_in, double orientation_in) {
	walls = std::vector<Wall>();
	pos = Coordinate(x_in, y_in);
	rotation = orientation_in;
	orientation.GetX() = cos(rotation * (3.14 / 180.0));
	orientation.GetY() = sin(rotation * (3.14 / 180.0));
	speed = 15;
}

//--------------------------------------------------------------------------------------

void Robot::SetSpeed(int speed_in) {
	speed = speed_in;
}

//--------------------------------------------------------------------------------------

void Robot::SetWalls(std::vector<Wall> &walls_in) {
	walls = walls_in;
}

//--------------------------------------------------------------------------------------

void Robot::AddSensor(Sensor &sensor) {
	sensor.AddWalls(walls);
	sensors.push_back(&sensor);
}

//--------------------------------------------------------------------------------------

void Robot::SetSprite(ofImage sprite_in) {
	sprite = sprite_in;
}

//--------------------------------------------------------------------------------------

void Robot::SetPosition(Coordinate position_in, double rotation_in) {
	pos = position_in;
	rotation = rotation_in;
	orientation.GetX() = cos(rotation * (3.14 / 180.0));
	orientation.GetY() = sin(rotation * (3.14 / 180.0));
}

//--------------------------------------------------------------------------------------

void Robot::Render(bool render_sensors) {
	ofPushMatrix();
	ofTranslate(pos.GetX(), pos.GetY(), 0);
	double angle = ((std::atan2(orientation.GetY(), orientation.GetX()) / PI) * 180.0);
	ofRotateDeg(angle);
	sprite.draw(-sprite.getWidth() / 2, -sprite.getHeight() / 2, 0);
	ofPopMatrix();

	if (render_sensors) {
		for (Sensor* sensor : sensors) {
			sensor->Render();
		}
	}

	if (Collided()) {
		collided = true;
	}
}

//--------------------------------------------------------------------------------------

void Robot::Move(double input) {
	if (!collided) {
		pos.GetX() = (pos.GetX() + input * speed * orientation.GetX());
		pos.GetY() = (pos.GetY() + input * speed * orientation.GetY());

		for (Sensor* sensor : sensors) {
			sensor->GetX() = pos.GetX();
			sensor->GetY() = pos.GetY();
		}
	}
}

//--------------------------------------------------------------------------------------

void Robot::Rotate(double input) {
	if (!collided) {
		rotation += speed * input;
		rotation = fmod(rotation, 360);
		orientation.GetX() = cos(rotation * (PI / 180.0));
		orientation.GetY() = sin(rotation * (PI / 180.0));

		double angle = speed * input * (PI / 180.0);
		for (Sensor* sensor : sensors) {
			sensor->GetRotation() += 15 * input;
			sensor->GetRotation() = fmod(sensor->GetRotation(), 360);
			sensor->GetOrientation().GetX() = cos(sensor->GetRotation() * (PI / 180.0));
			sensor->GetOrientation().GetY() = sin(sensor->GetRotation() * (PI / 180.0));
		}
	}
}

//--------------------------------------------------------------------------------------

bool Robot::Collided() {
	for (Wall wall : walls) {
		Coordinate beginning = wall.GetBeginning();
		Coordinate ending = wall.GetEnding();

		double dist_ab = Distance(beginning, ending);
		double dist_ac = Distance(beginning, pos);
		double dist_cb = Distance(pos, ending);

		if (std::abs(dist_ab - (dist_ac + dist_cb)) < 2) {
			return true;
		}

		continue;
	}
	return false;
}

//--------------------------------------------------------------------------------------

void Robot::Initialize() {
	Sensor* one = new Sensor(pos.GetX(), pos.GetY(), -80);
	Sensor* two = new Sensor(pos.GetX(), pos.GetY(), -100);
	Sensor* three = new Sensor(pos.GetX(), pos.GetY(), -45);
	Sensor* four = new Sensor(pos.GetX(), pos.GetY(), -135);
	Sensor* five = new Sensor(pos.GetX(), pos.GetY(), -63.5);
	Sensor* six = new Sensor(pos.GetX(), pos.GetY(), -116.5);
	Sensor* seven = new Sensor(pos.GetX(), pos.GetY(), -180);
	Sensor* eight = new Sensor(pos.GetX(), pos.GetY(), 0);
	Sensor* nine = new Sensor(pos.GetX(), pos.GetY(), 90);
	Sensor* ten = new Sensor(pos.GetX(), pos.GetY(), 45);
	Sensor* eleven = new Sensor(pos.GetX(), pos.GetY(), 135);

	AddSensor(*one);
	AddSensor(*two);
	AddSensor(*three);
	AddSensor(*four);
	AddSensor(*five);
	AddSensor(*six);
	AddSensor(*seven);
	AddSensor(*eight);
	AddSensor(*nine);
	AddSensor(*ten);
	AddSensor(*eleven);

}

//--------------------------------------------------------------------------------------

double Robot::GetSensorReading(int index) {
	return sensors[index]->Calculate_Distance();
}

//--------------------------------------------------------------------------------------

std::vector<double> Robot::GetSensorReadings() {
	std::vector<double> output;
	for (size_t i = 0; i < sensors.size(); ++i) {
		output.push_back(GetSensorReading(i));
	}
	return output;
}

//--------------------------------------------------------------------------------------

void Robot::Reset() {
	collided = false;
}

//--------------------------------------------------------------------------------------

Coordinate Robot::Get_Position() {
	return pos;
}

//--------------------------------------------------------------------------------------

Robot::~Robot() {
	for (Sensor* sensor : sensors) {
		delete sensor;
	}
}