#ifndef TOURNAMENT_H
#define TOURNAMENT_H
#include "Simulation.h"
#include "Brain.h"

class Tournament {
private:

	int mode;
	Sim* simulation;
	std::vector<Neural_Net> pred_population;
	std::vector<double> pred_fitnesses;
	std::vector<Neural_Net> prey_population;
	std::vector<double> prey_fitnesses;
	int population_size;
	int num_generations;
	int generation;

	double average_fitness;
	double best_fitness;
	bool evolving_preds;

	Neural_Net* best_prey;
	Neural_Net* best_pred;

	double mutation_rate;
	int iterations;
	Coordinate default_pred_position;
	Coordinate default_prey_position;

public:

	//Requires: nothing
	//Modifies: itself
	//Effects: Creates a default tournament with pop_size of 20 and 20 generations
	Tournament();

	//Requires: Sim* simulation, int pop_size and int num_generations
	//Modifies: itself
	//Effects: Creates a tournament with specified pop_size and and num_generations
	Tournament(int pop_size_in, int num_generations_in);

	//Requires: int
	//Modifies: itself
	//Effects: Sets the mode in which the robots neural networks will operate
	void Set_Mode(int mode_in);

	//Requires: double mutation_rate
	//Modifies: itself
	//Effects: Sets the mutation rate of the tournament to the input
	void Set_Mutation_Rate(double mutation_rate_in);

	//Requires: std::vector<int> hidden_layers
	//Modifies: itself
	//Effects: Initializes the population to a set of neural networks with random weights
	void Initialize_Population(int num_inputs_in, std::vector<int> &hidden_layers_in);

	//Requires: Robot robot
	//Modifies: Sim* simulation
	//Effects: Adds the robot to the simulation
	void Add_Robot(Robot &robot_in);

	//Requires: int num_robots
	//Modifies: Sim* simulation
	//Effects: Creates a simulation with num_robots number of robots
	void Initialize(int num_robots);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Renders the Simulation
	void Render();

	//Requires: nothing
	//Modifies: itself
	//Effects: Runs the tournament selection on the population of Neural Nets
	void Run_Tournament();

	//Requires: int tournament_size
	//Modifies: population
	//Effects: Performs tournament selection on the population of Neural_Networks
	std::vector<Neural_Net> Tournament_Selection(int tournament_size);

	//Requires: Robot robot
	//Modifies: nothing
	//Effects: Calculates the fitness of a predator robot at any particular moment in time
	double Calculate_Pred_Fitness(Robot* robot);

	//Requires: Robot robot
	//Modifies: nothing
	//Effects: Calculates the fitness of a pey robot at any particular moment in time
	double Calculate_Prey_Fitness(Robot* robot);

	//Requires: std::vector<Neural_Net> pred_population, std::vector<double> pred_fitnesses
	//Modifies: pred_population, pred_fitnesses
	//Effects: Selects the new predator population
	void Select_Pred_Population(std::vector<Neural_Net>& pred_population, std::vector<double> &pred_fitnesses);

	//Requires: std::vector<Neural_Net> prey_population, std::vector<double> prey_fitnesses
	//Modifies: prey_population, prey_fitnesses
	//Effects: Selects the new prey population
	void Select_Prey_Population(std::vector<Neural_Net>& prey_population, std::vector<double>& prey_fitnesses);

	//Requires: nothing
	//Modifies: nothing
	//Effects: Returns the generation of the tournament
	int Get_Generation();

	//Requires: nothing
	//Modifies: itself
	//Effects: Swaps the population being evolved
	void Swap_Population();

	~Tournament();
	
};

#endif // !TOURNAMENT_h
