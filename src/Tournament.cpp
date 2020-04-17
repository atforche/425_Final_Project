#include "Tournament.h"
#include "ofApp.h"

Tournament::Tournament() {
	simulation = NULL;
	population_size = 20;
	num_generations = 20;
	mutation_rate = 0.05;
	iterations = 100;
	default_robot_position = Coordinate(100, 700);
}

//--------------------------------------------------------------------------------------

Tournament::Tournament(int pop_size_in, int num_generations_in) {
	simulation = NULL;
	population_size = pop_size_in;
	num_generations = num_generations_in;
	mutation_rate = 0.05;
	iterations = 100;
	default_robot_position = Coordinate(100, 700);
}

//--------------------------------------------------------------------------------------

void Tournament::Set_Mutation_Rate(double mutation_rate_in) {
	mutation_rate = mutation_rate_in;
}

//--------------------------------------------------------------------------------------

void Tournament::Initialize_Population(int num_inputs_in, std::vector<int>& hidden_layers_in) {
	for (int i = 0; i < population_size; ++i) {
		population.push_back(Neural_Net(num_inputs_in, 2, hidden_layers_in));
	}
}

//--------------------------------------------------------------------------------------

double Tournament::Measure_Fitness(Neural_Net net) {
	//Assume only one robot for now
	Robot* robot = simulation->GetRobots()[0];
	std::vector<double> sensor_readings = robot->GetSensorReadings();
	std::vector<double> net_output = net.Execute(sensor_readings);
	robot->Move(net_output[0]);
	robot->Rotate(net_output[1]);
	simulation->Render();

	return 0;
}

//--------------------------------------------------------------------------------------

void Tournament::Add_Robot(Robot &robot_in) {
	simulation->Add_Robot(robot_in);
}

//--------------------------------------------------------------------------------------

void Tournament::Initialize(int num_robots) {
	simulation = new Sim(ofGetWindowWidth(), ofGetWindowHeight());
	simulation->Initialize();

	ofImage robot_img("sprite.png");
	robot_img.resize(40, 40);
	for (int i = 0; i < num_robots; ++i) {
		Robot* robot = new Robot(default_robot_position.GetX(), default_robot_position.GetY(), -90);
		robot->SetSprite(robot_img);
		Add_Robot(*robot);
		robot->Initialize();
	}
}

//--------------------------------------------------------------------------------------

void Tournament::Render() {
	simulation->Render();
}

//--------------------------------------------------------------------------------------

void Tournament::Run_Tournament() {
	
	static int iteration = 0;
	static int population_index = 0;
	static std::vector<Robot*> robots = simulation->GetRobots();

	//if (iteration > 100) {
	//	robot->SetPosition(default_robot_position, -90);
	//	robot->Reset();
	//	++population_index;
	//	iteration = 0;
	//	return;
	//}
	
	for (size_t i = 0; i < robots.size(); ++i) {
		std::vector<double> sensor_readings = robots[i]->GetSensorReadings();
		std::vector<double> net_output = population[i].Execute(sensor_readings);
		robots[i]->Move(net_output[0]);
		robots[i]->Rotate(net_output[1]);
	}
	Render();
	++iteration;
	
}

//--------------------------------------------------------------------------------------

Tournament::~Tournament() {
	delete simulation;
}