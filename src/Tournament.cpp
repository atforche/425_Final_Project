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
	mutation_rate = 0.10;
	iterations = 100;
	default_pred_position = Coordinate(200, 400);
	default_prey_position = Coordinate(600, 400);
	generation = 1;
	pred_fitnesses.resize(population_size);
	prey_fitnesses.resize(population_size);
	average_fitness = 0;
	best_fitness = 0;
	evolving_preds = true;
	num_cycles = 50;
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
	simulation = new Sim(800, 800);
	simulation->Initialize();
	simulation->Add_Wall(Wall(Coordinate(400, 300), Coordinate(400, 500)));

	ofImage robot_img("sprite.png");
	robot_img.resize(40, 40);
	for (int i = 0; i < num_robots; ++i) {
		//Add in the initial population of predators
		Robot* robot = new Robot(default_pred_position.GetX(), default_pred_position.GetY(), 90);
		robot->Get_Color() = ofColor(255, 0, 0);
		robot->Set_Pred(true);
		robot->SetSprite(robot_img);
		Add_Robot(*robot);
		robot->Initialize();
		robot->SetSpeed(10);
	}	

	for (int i = 0; i < num_robots; ++i) {
		//Add in the initial population of prey
		Robot* prey = new Robot(default_prey_position.GetX(), default_prey_position.GetY(), -90);
		prey->SetSprite(robot_img);
		prey->Get_Color() = ofColor(0, 0, 255);
		prey->Set_Pred(false);
		Add_Robot(*prey);
		prey->Initialize();
		prey->SetSpeed(15);
	}
}

//--------------------------------------------------------------------------------------

void Tournament::Render(bool render) {
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
	ofDrawBitmapString(message, 50, 50);
	ofDrawBitmapString(message0, 50, 75);
	ofDrawBitmapString(message2, 50, 100);
	ofDrawBitmapString(message3, 50, 125);
	
	
	if (!render) {
		if (generation % num_generations == 0) {
			simulation->Render();
			
		}
	}
	else {
		simulation->Render();
	}
}

//--------------------------------------------------------------------------------------

void Tournament::Run_Tournament(bool render) {
	static int cycles = 0;
	static bool paused = false;
	static int iteration = 0;
	static std::vector<Robot*> robots = simulation->GetRobots();

	if (cycles == num_cycles && !paused) {
		system("pause");
		paused = true;
	}
	if (cycles == num_cycles && iteration > iterations) {
		ofstream outfile1;
		ofstream outfile2;

		outfile1.open("robots_Wall/pred.txt");
		outfile2.open("robots_Wall/prey.txt");

		best_pred->Print(outfile1);
		best_prey->Print(outfile2);

		outfile1.close();
		outfile2.close();
		exit(0);
	}
	
	if (generation == num_generations && !paused && iteration > iterations) {
		++cycles;
	}

	/* SWAP WHICH POPULATION IS BEING EVOLVED*/
	if (generation > num_generations) {

		ofstream pred_outfile;
		ofstream prey_outfile;

		std::string pred = "robots_Wall/pred_";
		pred += to_string(cycles);
		pred += ".txt";

		std::string prey = "robots_Wall/prey_";
		prey += to_string(cycles);
		prey += ".txt";

		pred_outfile.open(pred);
		prey_outfile.open(prey);
		best_pred->Print(pred_outfile);
		best_prey->Print(prey_outfile);
		pred_outfile.close();
		prey_outfile.close();

		Swap_Population();
		generation = 0;
		mutation_rate = 0.1;
		paused = false;
	}

	/* SELECTING A NEW GENERATION ONLY HAPPENS EVERY iterations ITERATIONS*/
	if (iteration > iterations) {
		//default_pred_position = Coordinate((rand() % 600) + 100, (rand() % 600) + 100);
		//default_prey_position = Coordinate((rand() % 600) + 100, (rand() % 600) + 100);
		//Reset the robots begin evolved
		Reset_and_Select_Population(robots);
		iteration = 0;
		mutation_rate = mutation_rate - 0.005;
		return;
	}
	
	/* EVALUATING AND MOVING THE POPULATION THAT IS EVOLVING */
	Evaluate_Evolving_Population(robots);

	/* EVALUATING AND MOVING THE INDIVIDUAL THAT IS NOT EVOLVING */
	Evaluate_Static_Population(robots);

	/* RENDER THE SIMULATION */
	Render(render);
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

double Tournament::Calculate_Pred_Fitness(Robot* robot, int index) {
	double max_dist = 1131;
	if (robot->Collided()) {
		return -1000;
	}
	Coordinate prey_position = simulation->GetRobots().at(index + population_size)->Get_Position();
	double dist = Distance(robot->Get_Position(), prey_position);
	return max_dist - dist;
}

//--------------------------------------------------------------------------------------

double Tournament::Calculate_Prey_Fitness(Robot* robot, int index) {
	if (robot->Collided()) {
		return -10000;
	}
	else if (robot->Collided(0, simulation->GetRobots().at(index + population_size))) {
		return -1000000;
	}
	Coordinate pred_position = simulation->GetRobots().at(index + population_size)->Get_Position();
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
	pred_population = Tournament_Selection(5);
	for (size_t i = 1; i < pred_population.size(); ++i) {
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
	prey_population = Tournament_Selection(5);
	for (size_t i = 1; i < prey_population.size(); ++i) {
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

	ofImage robot_image("sprite.png");
	robot_image.resize(40, 40);

	if (evolving_preds) {
		//Create the population of prey to be evolved
		for (int i = 0; i < population_size; ++i) {
			//Robot* robot = new Robot(default_prey_position.GetX(), default_prey_position.GetY(), -90);
			//robot->SetSprite(prey_image);
			//Add_Robot(*robot);
			//robot->Initialize();

			robots[i]->SetSprite(robot_image);
			robots[i]->Reset();
			robots[i]->SetPosition(default_prey_position, -90);
			robots[i]->Get_Color() = ofColor(0, 0, 255);
			robots[i]->Set_Pred(false);
			robots[i]->SetSpeed(15);

			robots[i + population_size]->SetSprite(robot_image);
			robots[i + population_size]->SetPosition(default_pred_position, 90);
			robots[i + population_size]->Get_Color() = ofColor(255, 0, 0);
			robots[i + population_size]->Set_Pred(true);
			robots[i + population_size]->SetSpeed(10);
			robots[i + population_size]->Reset();
		}
		/*Robot* pred = new Robot(default_pred_position.GetX(), default_pred_position.GetY(), -90);
		pred->SetSprite(pred_image);
		Add_Robot(*pred);
		pred->Initialize();*/

		
	}

	else {
		//Create the population of predators to be evolved
		for (int i = 0; i < population_size; ++i) {
			/*Robot* robot = new Robot(default_pred_position.GetX(), default_pred_position.GetY(), -90);
			robot->SetSprite(pred_image);
			Add_Robot(*robot);
			robot->Initialize();*/

			robots[i]->SetSprite(robot_image);
			robots[i]->SetPosition(default_pred_position, 90);
			robots[i]->Reset();
			robots[i]->Get_Color() = ofColor(255, 0, 0);
			robots[i]->Set_Pred(true);
			robots[i]->SetSpeed(10);

			robots[i + population_size]->SetSprite(robot_image);
			robots[i + population_size]->SetPosition(default_prey_position, -90);
			robots[i + population_size]->Get_Color() = ofColor(0, 0, 255);
			robots[i + population_size]->Set_Pred(false);
			robots[i + population_size]->SetSpeed(15);
		}
		/*Robot* prey = new Robot(default_prey_position.GetX(), default_prey_position.GetY(), -90);
		prey->SetSprite(prey_image);
		Add_Robot(*prey);
		prey->Initialize();*/

		

	}

	evolving_preds = !evolving_preds;
}

//--------------------------------------------------------------------------------------

void Tournament::Reset_and_Select_Population(std::vector<Robot*>& robots) {
	for (size_t i = 0; i < population_size; ++i) {
		if (evolving_preds) {
			robots[i]->SetPosition(default_pred_position, 90);

			//Some Basic fitness sharing
			double sharing_sum = 0;
			for (size_t j = 0; j < pred_population.size(); ++j) {
				if (i == j) { continue; };
				sharing_sum += Neural_Net_Sharing(&pred_population[i], &pred_population[j]);
			}
			if (sharing_sum < 1) { sharing_sum = 1; };
			pred_fitnesses[i] = pred_fitnesses[i] / sharing_sum;
		}
		else {
			robots[i]->SetPosition(default_prey_position, -90);

			//Some basic fitness sharing
			double sharing_sum = 0;
			for (size_t j = 0; j < prey_population.size(); ++j) {
				if (i == j) { continue; };
				sharing_sum += Neural_Net_Sharing(&prey_population[i], &prey_population[j]);
			}
			if (sharing_sum < 1) { sharing_sum = 1; };
			prey_fitnesses[i] = prey_fitnesses[i] / sharing_sum;
		}
		robots[i]->Reset();

		//Reset the population not being evolved
		if (evolving_preds) {
			robots[i + population_size] ->SetPosition(default_prey_position, -90);
		}
		else {
			robots[i + population_size]->SetPosition(default_pred_position, 90);
		}
		robots[i + population_size]->Reset();
	}
	

	if (evolving_preds) {
		Select_Pred_Population(pred_population, pred_fitnesses);
	}
	else {
		Select_Prey_Population(prey_population, prey_fitnesses);
	}
}

//--------------------------------------------------------------------------------------

void Tournament::Evaluate_Evolving_Population(std::vector<Robot*>& robots) {
	std::vector<double> sensor_readings;
	for (size_t i = 0; i < population_size; ++i) {
		//Don't move the robot if it's collided with a wall
		if (!robots[i]->Get_Collided()) {
			//Creates the input vector depending on the mode selected for the robots
			if (mode == 0) {
				sensor_readings = { robots[i]->Get_Position().GetX(), robots[i]->Get_Position().GetY(), robots[i]->Get_Orientation() };
			}
			else if (mode == 1) {
				sensor_readings = robots[i]->GetSensorReadings();
			}
			else {
				std::vector<Robot*> robots_to_detect = { robots[i + population_size] };
				std::vector<std::vector<ofColor>> camera_output = robots[i]->Get_Camera_Output(robots_to_detect);
				Color_Filter(camera_output, robots[i]->Get_Pred());
				std::vector<std::vector<double>> convolution = Convolute_Image(camera_output);
				std::vector<double> pooled_output = Column_Pooling(convolution);
				//Map_Values(pooled_output);
				sensor_readings = pooled_output;

				/*ofImage image = Create_Image(camera_output);
				ofSetColor(255, 255, 255);
				image.draw(800, 500);

				ofImage grayscale;
				grayscale.allocate(pooled_output.size(), 1, OF_IMAGE_GRAYSCALE);
				for (int i = 0; i < pooled_output.size(); ++i) {
					grayscale.setColor(i, 0, ofColor(10 * pooled_output[i]));
				}
				grayscale.resize(268, 178);
				grayscale.draw(800, 300);*/

				if (mode == 3) {
					std::vector<double> additional_readings = robots[i]->GetSensorReadings();
					Map_Values(additional_readings);
					for (size_t j = 0; j < additional_readings.size(); ++j) {
						sensor_readings.push_back(additional_readings[j]);
					}
				}
			}
			std::vector<double> net_output;
			if (evolving_preds) {
				net_output = pred_population[i].Execute(sensor_readings);
			}
			else {
				net_output = prey_population[i].Execute(sensor_readings);
			}
			robots[i]->Rotate(net_output[1]);
			robots[i]->Move(net_output[0]);
		}

		if (evolving_preds) {
			pred_fitnesses[i] += Calculate_Pred_Fitness(robots[i], i);
		}
		else {
			prey_fitnesses[i] += Calculate_Prey_Fitness(robots[i], i);
		}
	}
}

//--------------------------------------------------------------------------------------

void Tournament::Evaluate_Static_Population(std::vector<Robot*>& robots) {
	std::vector<double> net_output;
	std::vector<double> sensor_readings;
	for (size_t i = 0; i < population_size; ++i) {
		if (!robots[i + population_size]->Get_Collided()) {
			if (mode == 0) {
				sensor_readings = { robots[i + population_size]->Get_Position().GetX(), robots[i + population_size]->Get_Position().GetY(), robots[i + population_size]->Get_Orientation() };
			}
			else if (mode == 1) {
				sensor_readings = robots[i + population_size]->GetSensorReadings();
			}
			else {
				//For now, only let the robot see the closest out of the other population
				int best_opposing_fitness_index;
				/*if (evolving_preds) {
					std::vector<double> pred_distances;
					for (int i = 0; i < population_size; ++i) {
						pred_distances.push_back(Distance(robots.back()->Get_Position(), robots[i]->Get_Position()));
					}
					best_opposing_fitness_index = distance(pred_distances.begin(), max_element(pred_distances.begin(), pred_distances.end()));
				}
				else {
					std::vector<double> prey_distances;
					for (int i = 0; i < population_size; ++i) {
						prey_distances.push_back(Distance(robots.back()->Get_Position(), robots[i]->Get_Position()));
					}
					best_opposing_fitness_index = distance(prey_distances.begin(), max_element(prey_distances.begin(), prey_distances.end()));
				}*/
				std::vector<Robot*> robots_to_detect = { robots[i] };
				//std::vector<Robot*> robots_to_detect(robots.begin(), robots.begin() + population_size);
				std::vector<std::vector<ofColor>> camera_output = robots[i + population_size]->Get_Camera_Output(robots_to_detect);
				Color_Filter(camera_output, robots[i + population_size]->Get_Pred());
				std::vector<std::vector<double>> convolution = Convolute_Image(camera_output);
				std::vector<double> pooled_output = Column_Pooling(convolution);
				//Map_Values(pooled_output);
				sensor_readings = pooled_output;

				/*ofImage image = Create_Image(camera_output);
				ofSetColor(255, 255, 255);
				image.draw(800, 200);

				ofImage grayscale;
				grayscale.allocate(pooled_output.size(), 1, OF_IMAGE_GRAYSCALE);
				for (int i = 0; i < pooled_output.size(); ++i) {
					grayscale.setColor(i, 0, ofColor(10 * pooled_output[i]));
				}
				grayscale.resize(268, 178);
				grayscale.draw(800, 0);*/

				if (mode == 3) {
					std::vector<double> additional_readings = robots[i + population_size]->GetSensorReadings();
					Map_Values(additional_readings);
					for (size_t i = 0; i < additional_readings.size(); ++i) {
						sensor_readings.push_back(additional_readings[i]);
					}
				}
			}

			if (evolving_preds) {
				net_output = best_prey->Execute(sensor_readings);
				robots[i + population_size]->Collided(0, robots[i]);
			}
			else {
				net_output = best_pred->Execute(sensor_readings);
			}
			robots[i + population_size]->Move(net_output[0]);
			robots[i + population_size]->Rotate(net_output[1]);
		}
	}
}

//--------------------------------------------------------------------------------------

void Tournament::Simulate_Brains_Initialize() {
	pred_population.clear();
	prey_population.clear();

	std::vector<int> hidden_layers = { 1,1 };
	Neural_Net test1(1, 1, hidden_layers);
	Neural_Net test2(1, 1, hidden_layers);
	pred_population.push_back(test1);
	prey_population.push_back(test2);

	delete simulation;
	simulation = new Sim(800, 800);
	simulation->Initialize();
	//simulation->Add_Wall(Wall(Coordinate(400, 100), Coordinate(400, 700)));

	ofImage robot_img("sprite.png");
	robot_img.resize(40, 40);
	//Add in the initial population of predators
	Robot* robot = new Robot(default_pred_position.GetX(), default_pred_position.GetY(), 90);
	robot->Get_Color() = ofColor(255, 0, 0);
	robot->Set_Pred(true);
	robot->SetSprite(robot_img);
	Add_Robot(*robot);
	robot->Initialize();
	robot->SetSpeed(10);

	//Add in a single prey
	Robot* prey = new Robot(default_prey_position.GetX(), default_prey_position.GetY(), -90);
	prey->SetSprite(robot_img);
	prey->Get_Color() = ofColor(0, 0, 255);
	prey->Set_Pred(false);
	Add_Robot(*prey);
	prey->Initialize();
	prey->SetSpeed(15);
}

//--------------------------------------------------------------------------------------

void Tournament::Simulate_Brains() {

	static ifstream pred_infile;
	static ifstream prey_infile;
	static int iteration = 0;
	static int cycle = 1;

	if (iteration == 0) {
		string pred = "robots_Wall/pred_";
		pred += to_string(cycle);
		pred += ".txt";

		string prey = "robots_Wall/prey_";
		prey += to_string(cycle);
		prey += ".txt";

		pred_infile.open(pred);
		prey_infile.open(prey);
		pred_population[0].Read(pred_infile);
		prey_population[0].Read(prey_infile);
		pred_infile.close();
		prey_infile.close();
		++iteration;
		return;
	}

	std::vector<Robot*> robots = simulation->GetRobots();

	if (iteration > 100) {
		iteration = 0;
		robots[0]->Reset();
		robots[0]->SetPosition(default_pred_position, 90);
		robots[1]->Reset();
		robots[1]->SetPosition(default_prey_position, -90);
		++generation;
		++cycle;
		evolving_preds = !evolving_preds;
		return;
	}

	
	for (int i = 0; i < 2; ++i) {
		if (!robots[i]->Get_Collided()) {
			std::vector<double> sensor_readings;
			//Creates the input vector depending on the mode selected for the robots
			if (mode == 0) {
				sensor_readings = { robots[i]->Get_Position().GetX(), robots[i]->Get_Position().GetY(), robots[i]->Get_Orientation() };
			}
			else if (mode == 1) {
				sensor_readings = robots[i]->GetSensorReadings();
			}
			else {
				std::vector<Robot*> robots_to_detect = { robots[!i] };
				std::vector<std::vector<ofColor>> camera_output = robots[i]->Get_Camera_Output(robots_to_detect);
				Color_Filter(camera_output, robots[i]->Get_Pred());
				std::vector<std::vector<double>> convolution = Convolute_Image(camera_output);
				std::vector<double> pooled_output = Column_Pooling(convolution);
				//Map_Values(pooled_output);
				sensor_readings = pooled_output;

				ofImage image = Create_Image(camera_output);
				ofSetColor(255, 255, 255);
				image.draw(800, 200 + 300 * i);

				ofImage grayscale;
				grayscale.allocate(pooled_output.size(), 1, OF_IMAGE_GRAYSCALE);
				for (int i = 0; i < pooled_output.size(); ++i) {
					grayscale.setColor(i, 0, ofColor( 10 * pooled_output[i]));
				}
				grayscale.resize(268, 178);
				grayscale.draw(800, 0 + 300 * i);

				if (mode == 3) {
					std::vector<double> additional_readings = robots[i]->GetSensorReadings();
					Map_Values(additional_readings);
					for (size_t j = 0; j < additional_readings.size(); ++j) {
						sensor_readings.push_back(additional_readings[j]);
					}
				}
			}
			std::vector<double> net_output;
			if (i == 0) {
				net_output = pred_population[0].Execute(sensor_readings);
			}
			else {
				net_output = prey_population[0].Execute(sensor_readings);
				robots[1]->Collided(0, robots[0]);
			}
			robots[i]->Move(net_output[0]);
			robots[i]->Rotate(net_output[1]);
		}
	}
	Render(true);
	++iteration;
}

//--------------------------------------------------------------------------------------

void Tournament::Test() {
	//std::vector<Robot*> robots = simulation->GetRobots();
	//std::vector<Robot*> test = { robots[0] };
	//std::vector<std::vector<ofColor>> output = robots.back()->Get_Camera_Output(test);
	////ofImage image = Create_Image(output);
	////Print_Image(output);
	////image.resize(400, 400);
	////image.draw(200, 200);

	//std::vector<std::vector<double>> convolution = Convolute_Image(output);

	///*for (size_t i = 0; i < convolution.size(); ++i) {
	//	for (size_t j = 0; j < convolution[0].size(); ++j) {
	//		std::cout << int(convolution[i][j]) << " ";
	//	}
	//	std::cout << '\n';
	//}*/

	//std::vector<double> pooled_output = Column_Pooling(convolution);
	//Map_Values(pooled_output);
	//for (size_t i = 0; i < pooled_output.size(); ++i) {
	//	std::cout << std::setprecision(3) << pooled_output[i] << " ";
	//}

	ofstream outfile;
	outfile.open("robots_Wall/one.txt");
	pred_population[0].Print(outfile);
	outfile.close();

	ifstream infile;
	infile.open("robots_Wall/one.txt");
	std::vector<int> hidden_layers = { 1 };
	Neural_Net test(1, 1, hidden_layers);
	test.Read(infile);

	double difference = Neural_Net_Distance(pred_population[0].Get_Layers(), test.Get_Layers());
	exit(0);
}

//--------------------------------------------------------------------------------------

Tournament::~Tournament() {
	delete simulation;
	delete best_prey;
	delete best_pred;
}