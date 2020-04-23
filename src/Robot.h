#ifndef ROBOT_H
#define ROBOT_H
#include "Coordinate.h"
#include "Orientation.h"
#include "ofApp.h"
#include "Sensor.h"
#include "Wall.h"
#include <vector>
#include <cmath>

class Sim;
class Wall;

class Robot {
private:

	Coordinate pos;
	Orientation orientation; //Indicates the rotation of the robot as a unit vector
	double rotation; //Indicates the rotation of the robot in degrees
	std::vector<Sensor*> sensors;
	int speed;
	ofImage sprite;
	std::vector<Wall> walls;
	bool collided = false;
	ofColor color;
	bool pred;

public:

	//Requires: nothing
	//Modifies: itself
	//Effects: Creates a robot object at (0,0) with default values
	Robot();

	//Requires: Sim* sim, x coordinate, y coordinate, orientation
	//Modifies: itself
	//Effects: Creates a robot object at (x_in, y_in) pointing toward orientation
	Robot(double x_in, double y_in, double orientation_in);

	//Requires: ofImage object
	//Modifies: itself
	//Effects: Sets the sprite of the robot to the inputted image
	void SetSprite(ofImage sprite_in);

	//Requires: Coordinate position_in
	//Modifies: itself
	//Effects: Sets the position of the robot to the input
	void SetPosition(Coordinate position_in, double rotation_in);

	//Requires: int speed_in
	//Modifies: itself
	//Effects: Sets the speed of the robot to speed_in
	void SetSpeed(int speed_in);

	//Requires: vector of Walls
	//Modifies: itself
	//Effects: Sets the walls that the robot must avoid
	void SetWalls(std::vector<Wall>& walls_in);

	//Requires: Sensor object
	//Modifies: itself
	//Effects: Adds the sensor object to the list of the robots sensors
	void AddSensor(Sensor &sensor);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Renders the robot and it's connected sensors
	void Render(bool render_sensors = false);

	//Requires: a double between -1 and 1
	//Modifies: itself
	//Effects: Moves the robot by some fraction of its speed forward or backward
	void Move(double input);

	//Requires: a double between -1 and 1
	//Modifies: itself
	//Effects: Rotates the robot by some fraction of its speed left or right
	void Rotate(double input);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Determines of the robot has collided with a wall
	bool Collided(double input = 0, Robot* robot = NULL);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Initializes the sensors on the robot
	void Initialize();

	//Requires: an integer index
	//Modifies: nothing
	//Effects: Returns the distance reading from a particular sensor
	double GetSensorReading(int index);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns a vector of distance readings from every sensor
	std::vector<double> GetSensorReadings();

	//Requires: nothing
	//Modifies: itself
	//Effects: Resets the robot after it has collided
	void Reset();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns the position of the Robot
	Coordinate Get_Position();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns the orientation of the Robot
	double Get_Orientation();

	//Requires: std::vector<Robot> robots
	//Modifies: nothing
	//Effects: Returns a 2d matrix representing output from a "camera module" mounted on the robot
	std::vector<std::vector<ofColor>> Get_Camera_Output(std::vector<Robot*> robots);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns the width of the robot's sprite
	double Get_Sprite_Width();

	//Requires: ofColor color_in
	//Modifies: itself
	//Effects: Returns a reference to the color of the robot
	ofColor& Get_Color();

	//Requires: bool pred_in
	//Modifies: itself
	//Effects: Sets whether or not a robot is a predator
	void Set_Pred(bool pred_in);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns whehter the robot is a predator
	bool Get_Pred();

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns whether or not the robot has collided
	bool Get_Collided();

	~Robot();
};

#endif /* ROBOT_H */
