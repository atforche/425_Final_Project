#include "Tournament.h"
#include "ofApp.h"

Tournament::Tournament() {
	simulation = NULL;
	population_size = 20;
	num_generations = 20;
	mutation_rate = 0.05;
	iterations = 100;
	default_robot_position = Coordinate(100, 700);
	generation = 1;
	fitnesses.resize(population_size);
}

//--------------------------------------------------------------------------------------

Tournament::Tournament(int pop_size_in, int num_generations_in) {
	simulation = NULL;
	population_size = pop_size_in;
	num_generations = num_generations_in;
	mutation_rate = 0.01;
	iterations = 100;
	default_robot_position = Coordinate(100, 700);
	generation = 1;
	fitnesses.resize(population_size);
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
	std::string message = "Generation: " + std::to_string(generation);
	ofDrawBitmapString(message, 50, 50);
	simulation->Render();
}

//--------------------------------------------------------------------------------------

void Tournament::Run_Tournament() {

	static int iteration = 0;
	static std::vector<Robot*> robots = simulation->GetRobots();

	if (iteration > iterations) {
		for (size_t i = 0; i < robots.size(); ++i) {
			robots[i]->SetPosition(default_robot_position, -90);
			robots[i]->Reset();
		}
		//Only select a new population if we're supposed to have a next generation
		if (generation < num_generations) {
			++generation;
			population = Tournament_Selection(5);
			for (size_t i = 0; i < population.size(); ++i) {
				population[i].Mutate_Network(mutation_rate);
			}
			fitnesses.clear();
			fitnesses.resize(population_size);
		}
		iteration = 0;
		return;
	}
	
	for (size_t i = 0; i < robots.size(); ++i) {
		fitnesses[i] += Calculate_Fitness(robots[i]);
		std::vector<double> sensor_readings = robots[i]->GetSensorReadings();
		std::vector<double> net_output = population[i].Execute(sensor_readings);
		robots[i]->Move(net_output[0]);
		robots[i]->Rotate(net_output[1]);
	}
	Render();
	++iteration;
	
}

//--------------------------------------------------------------------------------------

std::vector<Neural_Net> Tournament::Tournament_Selection(int tournament_size) {
	//Initialize a uniform random distribution between 0 and population_size
	std::default_random_engine generator(rand());
	std::uniform_int_distribution<int> distribution(0, population.size() - 1);

	std::vector<Neural_Net> new_population;
	std::vector<int> tournament_indexes;
	std::vector<double> tournament_fitnesses;

	for (size_t i = 0; i < population.size(); ++i) {
		for (int j = 0; j < tournament_size; ++j) {
			int index = distribution(generator);
			tournament_indexes.push_back(index);
			tournament_fitnesses.push_back(fitnesses[index]);
		}
		auto iterator = std::max_element(tournament_fitnesses.begin(), tournament_fitnesses.end());
		int max_index = std::distance(tournament_fitnesses.begin(), iterator);
		new_population.push_back(population[tournament_indexes[max_index]]);
		tournament_indexes.clear();
		tournament_fitnesses.clear();
	}

	return new_population;
}

//--------------------------------------------------------------------------------------

double Tournament::Calculate_Fitness(Robot* robot) {
	if (robot->Collided()) {
		return -1000000;
	}
	Coordinate target(700, 100);
	double dist = Distance(target, robot->Get_Position());
	if (dist == 0) {
		dist = 0.0001;
	}
	return 1 / dist;
}

//--------------------------------------------------------------------------------------

Tournament::~Tournament() {
	delete simulation;
}