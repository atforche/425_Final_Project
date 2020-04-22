#include "Brain.h"

void Random_Brain(Robot& robot) {

	std::default_random_engine generator;
	generator.seed(rand());
	std::uniform_real_distribution<double> distribution(-1, 1);

	double move = distribution(generator);
	double rotation = distribution(generator);

	robot.Move(move);
	robot.Rotate(rotation);
}

//--------------------------------------------------------------------------------------

void Better_Brain(Robot& robot) {
	// Front Sensors = 1,2,3,4,5,6
	// Side Sensors = 7,8
	// Rear Sensors = 9,10,11

	static std::vector<double> previous_moves = {1,0};

	double move = previous_moves[0];
	double rotation = previous_moves[1];
	previous_moves.pop_back();
	previous_moves.pop_back();

	bool front_hazard = false;
	bool left_hazard = false;
	bool right_hazard = false;
	bool back_hazard = false;

	double threshold = 100;
	for (int i = 0; i < 7; ++i) {
		if (robot.GetSensorReading(i) < threshold) {
			front_hazard = true;
		}
	}

	if (robot.GetSensorReading(6) < threshold) {
		left_hazard = true;
	}

	if (robot.GetSensorReading(7) < threshold) {
		right_hazard = true;
	}

	for (int i = 9; i < 11; ++i) {
		if (robot.GetSensorReading(i) < threshold) {
			back_hazard = true;
		}
	}

	if (left_hazard && front_hazard) {
		move = 0.25;
		rotation = 1;
	}
	else if (front_hazard) {
		move = 0.25;
		rotation = -1;
	}
	else {
		move = 0.5;
		rotation = 0;
	}

	previous_moves.push_back(move);
	previous_moves.push_back(rotation);

	robot.Move(move);
	robot.Rotate(rotation);
}

//--------------------------------------------------------------------------------------

Neural_Net::Neural_Net(int num_inputs_in, int num_outputs_in, std::vector<int>& layer_node_counts_in = std::vector<int>()) {
	num_inputs = num_inputs_in;
	num_outputs = num_outputs_in;
	layer_node_counts = layer_node_counts_in;
	layers = { {{}} };

	//Seed the random number generator
	std::default_random_engine generator(rand());
	std::uniform_real_distribution<double> distribution(-1, 1);

	layer_node_counts.push_back(num_outputs);
	layers.resize(layer_node_counts.size());
	int last_num_neurons = num_inputs;
	for (size_t i = 0; i < layer_node_counts.size(); ++i) {
		layers[i].resize(last_num_neurons + 1);
		for (int j = 0; j < last_num_neurons + 1; ++j) {
			layers[i][j].resize(layer_node_counts[i]);
			for (int k = 0; k < layer_node_counts[i]; ++k) {
				layers[i][j][k] = distribution(generator);
			}
		}
		last_num_neurons = layer_node_counts[i];
	}
	layer_node_counts.pop_back();
}

//----------------------------------------------------------------------------------------------------------------------------------

double Neural_Net::Activation_Function(double x) {
	double numerator = exp(2 * x) - 1;
	double denominator = exp(2 * x) + 1;
	if (!isfinite(numerator) && !isfinite(denominator)) {
		return 1;
	}
	else if (!isfinite(denominator)) {
		return 0;
	}
	else if (!isfinite(numerator)) {
		return 1;
	}
	else if (denominator == 0) {
		return 1;
	}
	return (numerator / denominator);
}

//----------------------------------------------------------------------------------------------------------------------------------

Neural_Net* Neural_Net::Deep_Copy() {
	Neural_Net* new_net = new Neural_Net(num_inputs, num_outputs, layer_node_counts);
	new_net->layers = layers;
	return new_net;
}

//----------------------------------------------------------------------------------------------------------------------------------

std::vector<double> Neural_Net::Execute(std::vector<double>& input_vector) {
	assert(input_vector.size() == static_cast<size_t>(num_inputs));
	std::vector<std::vector<double>> output;
	std::vector<std::vector<double>> next_input_vector = { {} };
	next_input_vector[0] = input_vector;
	for (size_t i = 0; i < layers.size(); ++i) {
		next_input_vector[0].push_back(1);

		output = Matrix_Multiplication(next_input_vector, layers[i]);

		for (size_t i = 0; i < output[0].size(); ++i) {
			output[0][i] = Activation_Function(output[0][i]);
		}
		next_input_vector = output;
	}

	return output[0];
}

//----------------------------------------------------------------------------------------------------------------------------------

void Neural_Net::Mutate_Network(double mutation_rate) {
	std::default_random_engine generator(rand());
	std::uniform_real_distribution<double> distribution(0, 1);
	std::uniform_real_distribution<double> distribution2(-1, 1);
	for (size_t i = 0; i < layers.size(); ++i) {
		for (size_t j = 0; j < layers[i].size(); ++j) {
			for (size_t k = 0; k < layers[i][j].size(); ++k) {
				double random = distribution(generator);
				if (random < mutation_rate) {
					layers[i][j][k] += distribution2(generator);
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------

std::vector<std::vector<std::vector<double>>> Neural_Net::Get_Layers() {
	return layers;
}

//--------------------------------------------------------------------------------------

void Neural_Net::Print(ofstream& outfile) {

	outfile << num_inputs << '\n';
	outfile << num_outputs << '\n';
	outfile << layer_node_counts.size() << '\n';
	for (size_t i = 0; i < layer_node_counts.size(); ++i) {
		outfile << layer_node_counts[i] << " ";
	}
	outfile << '\n';

	for (size_t i = 0; i < layers.size(); ++i) {
		for (size_t j = 0; j < layers[i].size(); ++j) {
			for (size_t k = 0; k < layers[i][j].size(); ++k) {
				outfile << layers[i][j][k] << " ";
			}
		}
		outfile << '\n';
	}
}

//--------------------------------------------------------------------------------------

void Neural_Net::Read(ifstream& infile) {
	infile >> num_inputs;
	infile >> num_outputs;
	int hidden_layers;
	infile >> hidden_layers;
	layer_node_counts.resize(hidden_layers);
	for (int i = 0; i < hidden_layers; ++i) {
		infile >> layer_node_counts[i];
	}
	layers.resize(hidden_layers + 1);
	for (size_t i = 0; i < layers.size(); ++i) {
		if (i == layers.size() - 1) {
			layers[0].resize(num_inputs + 1);
		}
		else {
			layers[i + 1].resize(layer_node_counts[i] + 1);
		}
	}

	for (size_t i = 0; i < layers.size(); ++i) {
		for (size_t j = 0; j < layers[i].size(); ++j) {
			if (i != layers.size() - 1) {
				layers[i][j].resize(layer_node_counts[i]);
			}
			else {
				layers[i][j].resize(num_outputs);
			}
		}
	}

	for (int i = 0; i < num_inputs + 1; ++i) {
		for (int j = 0; j < layer_node_counts[0]; ++j) {
			infile >> layers[0][i][j];
		}
	}

	for (int i = 0; i < layer_node_counts[0] + 1; ++i) {
		for (int j = 0; j < layer_node_counts[1]; ++j) {
			infile >> layers[1][i][j];
		}
	}

	for (int i = 0; i < layer_node_counts[1] + 1; ++i) {
		for (int j = 0; j < num_outputs; ++j) {
			infile >> layers[2][i][j];
		}
	}
}