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
	ofSetColor(color);
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
	if (!Collided(input) && !collided) {
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
			sensor->Rotate(15 * input);
		}
	}
}

//--------------------------------------------------------------------------------------

bool Robot::Collided(double input, Robot* robot) {
	for (Wall wall : walls) {
		Coordinate beginning = wall.GetBeginning();
		Coordinate ending = wall.GetEnding();

		double dist_ab = Distance(beginning, ending);
		double dist_ac = Distance(beginning, pos);
		double dist_cb = Distance(pos, ending);

		if (std::abs(dist_ab - (dist_ac + dist_cb)) < 2) {
			return true;
		}
	}

	//Checks to see if the robot will move through a wall on its next move
	if (input != 0) {
		Sensor will_collide(pos.GetX(), pos.GetY(), rotation);
		will_collide.AddWalls(walls);
		double min_dist = will_collide.Calculate_Distance();
		if (min_dist <= (input * speed)) {
			return true;
		}
	}

	if (robot) {
		Coordinate pred_position = robot->Get_Position();
		double distance = Distance(pos, pred_position);
		if (distance < 40) {
			collided = true;
			return true;
		}
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

double Robot::Get_Orientation() {
	return rotation;
}

//--------------------------------------------------------------------------------------

std::vector<std::vector<ofColor>> Robot::Get_Camera_Output(std::vector<Robot*> &robots) {

	std::vector<int> output;
	std::vector<ofColor> color_readings;
	std::vector<double> distance_readings;

	//First, convert the robots that need to be detected into wall objects
	//Each robots will be represented as a cross_shaped pair of walls for simplicity
	std::vector<Wall> temp_walls = walls;

	for (size_t i = 0; i < robots.size(); ++i) {
		Coordinate position = robots[i]->Get_Position();
		double sprite_width = robots[i]->Get_Sprite_Width();
		Wall vertical(Coordinate(position.GetX(), position.GetY() - sprite_width / 2), Coordinate(position.GetX(), position.GetY() + sprite_width / 2));
		vertical.Get_Color() = robots[i]->Get_Color();
		Wall horizontal(Coordinate(position.GetX() - sprite_width / 2, position.GetY()), Coordinate(position.GetX() + sprite_width / 2, position.GetY()));
		horizontal.Get_Color() = robots[i]->Get_Color();
		temp_walls.push_back(vertical);
		temp_walls.push_back(horizontal);
	}

	//Then, use 30 samples of the 45 degree FOV in front of the robot
	//Constructing a vector from what each sample returns

	//Initialize the camera sensor
	double FOV = 135;
	Sensor camera(pos.GetX(), pos.GetY(), fmod(rotation - (FOV / 2), 360));
	camera.AddWalls(temp_walls);

	//Get 180 samples from the 90 degree FOV in front of the robot (every 1.5 degrees)
	for (int i = 0; i < 3 * FOV; ++i) {
		ofColor color_reading;
		double distance_reading = camera.Camera_Calculate_Distance(color_reading);
		color_readings.push_back(color_reading);
		distance_readings.push_back(distance_reading);
		camera.Rotate(0.33);
	}

	//Then, convert the sample vector into a 30x30 image type thingy
	ofColor sky_color(0, 102, 204);
	ofColor ground_color(255, 204, 153);

	std::vector<std::vector<ofColor>> image;
	std::vector<ofColor> column;
	column.resize(60);
	for (int i = 0; i < distance_readings.size(); ++i) {
		int num_pixels = Map_Distance_To_Pixels(distance_readings[i]);
		for (int j = (60 / 2) - (num_pixels / 2) - 1; j < (60 / 2) + (num_pixels / 2) - 1; ++j) {
			column[j] = color_readings[i];
		}
		for (size_t i = 0; i < column.size(); ++i) {
			if (column[i] == ofColor(255, 255, 255) && i < (60 / 2) ) {
				column[i] = sky_color;
			}
			else if (column[i] == ofColor(255, 255, 255)) {
				column[i] = ground_color;
			}
		}
		image.push_back(column);
		column.clear();
		column.resize(60);
	}

	//Print_Image(image);
	return image;
}

//--------------------------------------------------------------------------------------

double Robot::Get_Sprite_Width() {
	return sprite.getWidth();
}

//--------------------------------------------------------------------------------------

ofColor& Robot::Get_Color() {
	return color;
}

//--------------------------------------------------------------------------------------

void Robot::Set_Pred(bool pred_in) {
	pred = pred_in;
}

//--------------------------------------------------------------------------------------

bool Robot::Get_Pred() {
	return pred;
}

//--------------------------------------------------------------------------------------

bool Robot::Get_Collided() {
	return collided;
}

//--------------------------------------------------------------------------------------

Robot::~Robot() {
	for (Sensor* sensor : sensors) {
		delete sensor;
	}
}