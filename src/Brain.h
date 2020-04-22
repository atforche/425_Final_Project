#ifndef BRAIN_H
#define BRAIN_H
#include "Robot.h"
#include "Helpers.h"
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <ctime>
#include <random>

//Requires: a robot object
//Modifies: robot
//Effects: Moves and rotates the robot in a random way
void Random_Brain(Robot& robot);

//Requires: a robot objcet
//Modifies: robot
//Effects: Moves and rotates the robot in an effort to avoid walls
void Better_Brain(Robot& robot);

class Neural_Net {

private:
    int num_inputs;
    std::vector<int> layer_node_counts;
    int num_outputs;
    std::vector<std::vector<std::vector<double>>> layers;

public:

    //Requires: int num_inputs, int num_outputs, std::vector<int> layer_node_counts
    //Modifies: itself
    //Effects: Constructs a Neural_Net with num_inputs inputs, num_outputs ouputs, and layer_node_counts hidden layers
    Neural_Net(int num_inputs_in, int num_outputs_in, std::vector<int>& layer_node_counts_in);

    //Requires: double x
    //Modifies: nothing
    //Effects: Returns the result of the activation function
    double Activation_Function(double x);

    //Requires: nothing
    //Modifies: nothing
    //Effects: Returns a deep copy of itself
    Neural_Net* Deep_Copy();

    //Requires:: std::vector<double> input_vector
    //Modifies: nothing
    //Effects: Executes the Neural_Net for a given input_vector
    std::vector<double> Execute(std::vector<double>& input_vector);

    //Requires: double mutation_rate
    //Modifies: itself
    //Effects: Mutates the layers of the neural network with some given probability
    void Mutate_Network(double mutation_rate);

    //Requires: nothing
    //Modifies: nothing
    //Effects: Returns the layers of the neural net
    std::vector<std::vector<std::vector<double>>> Get_Layers();

    //Requires: ofstream& outfile
    //Modifies: outfile
    //Effects: Stores the Neural_Net into outfile
    void Print(ofstream &outfile);

    //Requires: ifstream& infile
    //Modifies: itself
    //Effects: Returns the Neural_Net specified in the file
    void Read(ifstream& infile);

};

#endif /* BRAIN_H */
