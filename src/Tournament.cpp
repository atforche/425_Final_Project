#include "Tournament.h"
#include "ofApp.h"

Tournament::Tournament() {
	simulation = NULL;
	population_size = 20;
	num_generations = 20;
	mutation_rate = 0.05;
	iterations = 100;
	default_pred_position = Coordinate(700, 100);
	default_prey_position = Coordinate(100, 700);
	generation = 1;
	pred_fitnesses.resize(population_size);
	prey_fitnesses.resize(population_size);
}

//--------------------------------------------------------------------------------------

Tournament::Tournament(int pop_size_in, int num_generations_in) {
	simulation = NULL;
	population_size = pop_size_in;
	num_generations = num_generations_in;
	mutation_rate = 0.01;
	iterations = 100;
	default_pred_position = Coordinate(700, 100);
	default_prey_position = Coordinate(400, 400);
	generation = 1;
	pred_fitnesses.resize(population_size);
	prey_fitnesses.resize(population_size);
	average_fitness = 0;
	best_fitness = 0;
	evolving_preds = true;
}

//--------------------------------------------------------------------------------------

void Tournament::Set_Mode(int mode_in) {
	mode = mode_in;
}

//--------------------------------------------------------------------------------------

void Tournament::Set_Mutation_Rate(double mutation_rate_in) {
	mutation_rate = mutation_rate_in;
}

//--------------------------------------------------------------------------------------

void Tournament::Initialize_Population(int num_inputs_in, std::vector<int>& hidden_layers_in) {
	for (int i = 0; i < population_size; ++i) {
		prey_population.push_back(Neural_Net(num_inputs_in, 2, hidden_layers_in));
		pred_population.push_back(Neural_Net(num_inputs_in, 2, hidden_layers_in));
	}
	best_prey = new Neural_Net(num_inputs_in, 2, hidden_layers_in);
	best_pred = new Neural_Net(num_inputs_in, 2, hidden_layers_in);
}

//--------------------------------------------------------------------------------------

void Tournament::Add_Robot(Robot &robot_in) {
	simulation->Add_Robot(robot_in);
}

//--------------------------------------------------------------------------------------

void Tournament::Initialize(int num_robots) {
	simulation = new Sim(ofGetWindowWidth(), ofGetWindowHeight());
	simulation->Initialize();
	//simulation->Add_Wall(Wall(Coordinate(400, 100), Coordinate(400, 700)));

	ofImage robot_img("pred.png");
	robot_img.resize(40, 40);
	for (int i = 0; i < num_robots; ++i) {
		//Add in the initial population of predators
		Robot* robot = new Robot(default_pred_position.GetX(), default_pred_position.GetY(), -90);
		robot->SetSprite(robot_img);
		Add_Robot(*robot);
		robot->Initialize();
	}
	//Add in a single prey
	Robot* prey = new Robot(default_prey_position.GetX(), default_prey_position.GetY(), -90);
	ofImage sprite("sprite.png");
	sprite.resize(40, 40);
	prey->SetSprite(sprite);
	Add_Robot(*prey);
	prey->Initialize();
}

//--------------------------------------------------------------------------------------

void Tournament::Render() {
	std::string evolving;
	if (evolving_preds) {
		evolving = "Predators";
	}
	else {
		evolving = "Prey";
	}
	std::string message0 = "Evolving: " + evolving;;
	std::string message = "Generation: " + std::to_string(generation);
	std::string message2 = "Average Fitness: " + std::to_string(average_fitness);
	std::string message3 = "Best Fitness: " + std::to_string(best_fitness);
	ofDrawBitmapString(message0, 50, 50);
	ofDrawBitmapString(message, 50, 75);
	ofDrawBitmapString(message2, 50, 100);
	ofDrawBitmapString(message3, 50, 125);
	if (generation % num_generations == 0) {
		simulation->Render();
	}
}

//--------------------------------------------------------------------------------------

void Tournament::Run_Tournament() {

	static int iteration = 0;
	static std::vector<Robot*> robots = simulation->GetRobots();

	/* SWAP WHICH POPULATION IS BEING EVOLVED*/
	if (generation > num_generations) {
		Swap_Population();
		generation = 0;
	}

	/* SELECTING A NEW GENERATION ONLY HAPPENS EVERY 100 ITERATIONS*/
	if (iteration > iterations) {
		//Reset the robots begin evolved
		for (size_t i = 0; i < population_size; ++i) {
			if (evolving_preds) {
				robots[i]->SetPosition(default_pred_position, -90);
			}
			else {
				robots[i]->SetPosition(default_prey_position, -90);
			}
			robots[i]->Reset();
		}
		//Reset the single robot not being evolved
		if (evolving_preds) {
			robots.back()->SetPosition(default_prey_position, -90);
		}
		else {
			robots.back()->SetPosition(default_pred_position, -90);
		}
		robots.back()->Reset();

		if (evolving_preds) {
			Select_Pred_Population(pred_population, pred_fitnesses);
		}
		else {
			Select_Prey_Population(prey_population, prey_fitnesses);
		}
		iteration = 0;
		return;
	}
	
	/* EVALUATING AND MOVING THE POPULATION THAT IS EVOLVING */
	for (size_t i = 0; i < population_size; ++i) {
		//Don't move the robot if it's collided with a wall
		if (!robots[i]->Collided()) {
			std::vector<double> sensor_readings;
			//Creates the input vector depending on the mode selected for the robots
			if (mode == 0) {
				sensor_readings = { robots[i]->Get_Position().GetX(), robots[i]->Get_Position().GetY(), robots[i]->Get_Orientation() };
			}
			else if (mode == 1) {
				sensor_readings = robots[i]->GetSensorReadings();
			}
			else if (mode == 2) {

			}
			 
			
			std::vector<double> net_output;
			if (evolving_preds) {
				net_output = pred_population[i].Execute(sensor_readings);
			}
			else {
				net_output = prey_population[i].Execute(sensor_readings);
			}
			robots[i]->Move(net_output[0]);
			robots[i]->Rotate(net_output[1]);
		}

		//Now evaluate the fitness of the robot
		if (robots[i]->Collided()) {
			if (evolving_preds) {
				pred_fitnesses[i] = 0;
			}
			else {
				prey_fitnesses[i] = 0;
			}
		}
		else if (evolving_preds) {
			pred_fitnesses[i] += Calculate_Pred_Fitness(robots[i]);
		}
		else {
			prey_fitnesses[i] += Calculate_Prey_Fitness(robots[i]);
		}
	}

	/* EVALUATING AND MOVING THE INDIVIDUAL THAT IS NOT EVOLVING */
	std::vector<double> net_output;
	std::vector<double> sensor_readings;
	if (mode == 0) {
		sensor_readings = { robots.back()->Get_Position().GetX(), robots.back()->Get_Position().GetY(), robots.back()->Get_Orientation() };
	}
	else if (mode == 1) {
		sensor_readings = robots.back()->GetSensorReadings();
	}
	else if (mode == 2) {
		
	}

	if (evolving_preds) {
		net_output = best_prey->Execute(sensor_readings);
	}
	else {
		net_output = best_pred->Execute(sensor_readings);
	}
	robots.back()->Move(net_output[0]);
	robots.back()->Rotate(net_output[1]);

	//Execute the Neural Network
	Render();
	++iteration;
	
}

//--------------------------------------------------------------------------------------

std::vector<Neural_Net> Tournament::Tournament_Selection(int tournament_size) {
	//Initialize a uniform random distribution between 0 and population_size
	std::default_random_engine generator(rand());
	std::uniform_int_distribution<int> distribution(0, pred_population.size() - 1);

	std::vector<Neural_Net> new_population;
	std::vector<int> tournament_indexes;
	std::vector<double> tournament_fitnesses;

	//Guarantee that the best robot from each generation gets carried on
	if (evolving_preds) {
		auto iterator = std::max_element(pred_fitnesses.begin(), pred_fitnesses.end());
		int max_index = std::distance(pred_fitnesses.begin(), iterator);
		new_population.push_back(pred_population[max_index]);
		best_pred = pred_population[max_index].Deep_Copy();
	}
	else {
		auto iterator = std::max_element(prey_fitnesses.begin(), prey_fitnesses.end());
		int max_index = std::distance(prey_fitnesses.begin(), iterator);
		new_population.push_back(prey_population[max_index]);
		best_prey = prey_population[max_index].Deep_Copy();
	}

	for (size_t i = 0; i < population_size - 1; ++i) {
		for (int j = 0; j < tournament_size; ++j) {
			int index = distribution(generator);
			tournament_indexes.push_back(index);
			if (evolving_preds) {
				tournament_fitnesses.push_back(pred_fitnesses[index]);
			}
			else {
				tournament_fitnesses.push_back(prey_fitnesses[index]);
			}
		}
		auto iterator = std::max_element(tournament_fitnesses.begin(), tournament_fitnesses.end());
		int max_index = std::distance(tournament_fitnesses.begin(), iterator);
		if (evolving_preds) {
			new_population.push_back(pred_population[tournament_indexes[max_index]]);
		}
		else {
			new_population.push_back(prey_population[tournament_indexes[max_index]]);
		}
		tournament_indexes.clear();
		tournament_fitnesses.clear();
	}

	return new_population;
}

//--------------------------------------------------------------------------------------

double Tournament::Calculate_Pred_Fitness(Robot* robot) {
	Coordinate prey_position = simulation->GetRobots().back()->Get_Position();
	double dist = Distance(robot->Get_Position(), prey_position);
	if (dist == 0) { dist = 0.00001; };
	return 1 / dist;
}

//--------------------------------------------------------------------------------------

double Tournament::Calculate_Prey_Fitness(Robot* robot) {
	Coordinate pred_position = simulation->GetRobots().back()->Get_Position();
	double dist = Distance(robot->Get_Position(), pred_position);
	return dist;
}

//--------------------------------------------------------------------------------------

void Tournament::Select_Pred_Population(std::vector<Neural_Net>& pred_population, std::vector<double> &pred_fitnesses) {
	double total_fitness = std::accumulate(pred_fitnesses.begin(), pred_fitnesses.end(), 0);
	average_fitness = total_fitness / pred_fitnesses.size();
	auto iterator = std::max_element(pred_fitnesses.begin(), pred_fitnesses.end());
	best_fitness = *iterator;
	++generation;
	pred_population = Tournament_Selection(20);
	for (size_t i = 0; i < pred_population.size(); ++i) {
		pred_population[i].Mutate_Network(mutation_rate);
	}
	pred_fitnesses.clear();
	pred_fitnesses.resize(population_size);
}

//--------------------------------------------------------------------------------------

void Tournament::Select_Prey_Population(std::vector<Neural_Net>& prey_population, std::vector<double>& prey_fitnesses) {
	double total_fitness = accumulate(prey_fitnesses.begin(), prey_fitnesses.end(), 0);
	average_fitness = total_fitness / prey_fitnesses.size();
	auto iterator = std::max_element(prey_fitnesses.begin(), prey_fitnesses.end());
	best_fitness = *iterator;
	++generation;
	prey_population = Tournament_Selection(20);
	for (size_t i = 0; i < prey_population.size(); ++i) {
		prey_population[i].Mutate_Network(mutation_rate);
	}
	prey_fitnesses.clear();
	prey_fitnesses.resize(population_size);
}

//--------------------------------------------------------------------------------------

int Tournament::Get_Generation() {
	return generation;
}

//--------------------------------------------------------------------------------------

void Tournament::Swap_Population() {
	//simulation->Reset();
	std::vector<Robot*> robots = simulation->GetRobots();

	ofImage prey_image("sprite.png");
	prey_image.resize(40, 40);
	ofImage pred_image("pred.png");
	pred_image.resize(40, 40);

	if (evolving_preds) {
		//Create the population of prey to be evolved
		for (int i = 0; i < population_size; ++i) {
			//Robot* robot = new Robot(default_prey_position.GetX(), default_prey_position.GetY(), -90);
			//robot->SetSprite(prey_image);
			//Add_Robot(*robot);
			//robot->Initialize();

			robots[i]->SetSprite(prey_image);
			robots[i]->Reset();
			robots[i]->SetPosition(default_prey_position, -90);
		}
		/*Robot* pred = new Robot(default_pred_position.GetX(), default_pred_position.GetY(), -90);
		pred->SetSprite(pred_image);
		Add_Robot(*pred);
		pred->Initialize();*/

		robots.back()->SetSprite(pred_image);
		robots.back()->SetPosition(default_pred_position, -90);
	}

	else {
		//Create the population of predators to be evolved
		for (int i = 0; i < population_size; ++i) {
			/*Robot* robot = new Robot(default_pred_position.GetX(), default_pred_position.GetY(), -90);
			robot->SetSprite(pred_image);
			Add_Robot(*robot);
			robot->Initialize();*/

			robots[i]->SetSprite(pred_image);
			robots[i]->SetPosition(default_pred_position, -90);
			robots[i]->Reset();
		}
		/*Robot* prey = new Robot(default_prey_position.GetX(), default_prey_position.GetY(), -90);
		prey->SetSprite(prey_image);
		Add_Robot(*prey);
		prey->Initialize();*/

		robots.back()->SetSprite(prey_image);
		robots.back()->SetPosition(default_prey_position, -90);
	}

	evolving_preds = !evolving_preds;
}

//--------------------------------------------------------------------------------------

Tournament::~Tournament() {
	delete simulation;
	delete best_prey;
	delete best_pred;
}