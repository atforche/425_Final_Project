#include "Simulation.h"

Sim::Sim() {
	halted = false;
	window_width = 800;
	window_height = 800;
}

//--------------------------------------------------------------------------------------

Sim::Sim(double window_width_in, double window_height_in) {
	halted = false;
	window_width = window_width_in;
	window_height = window_height_in;
}

//--------------------------------------------------------------------------------------

void Sim::Add_Robot(Robot &robot) {
	robot.SetWalls(walls);
	robots.push_back(&robot);
}

//--------------------------------------------------------------------------------------

void Sim::Add_Wall(Wall& wall) {
	walls.push_back(wall);
}

//--------------------------------------------------------------------------------------

void Sim::Render(bool sensors) {
	for (Wall wall : walls) {
		wall.Render();
	}
	for (Robot* robot : robots) {
		robot->Render(sensors);
	}
}

//--------------------------------------------------------------------------------------

bool Sim::IsHalted() {
	return halted;
}

//--------------------------------------------------------------------------------------

void Sim::Initialize() {
	Coordinate sw(0, 0);
	Coordinate nw(0, window_height);
	Coordinate se(window_width, 0);
	Coordinate ne(window_width, window_height);

	this->Add_Wall(Wall(sw, nw));
	this->Add_Wall(Wall(sw, se));
	this->Add_Wall(Wall(se, ne));
	this->Add_Wall(Wall(nw, ne));

}

//--------------------------------------------------------------------------------------

std::vector<Wall> Sim::GetWalls() {
	return walls;
}

//--------------------------------------------------------------------------------------

std::vector<Robot*> Sim::GetRobots() {
	return robots;
}

//--------------------------------------------------------------------------------------

void Sim::Reset() {
	for (size_t i = 0; i < robots.size(); ++i) {
		delete robots[i];
		robots[i] = NULL;
	}
	robots.clear();
}

//--------------------------------------------------------------------------------------

Sim::~Sim() {
	for (size_t i = 0; i < robots.size(); ++i) {
		delete robots[i];
	}
}