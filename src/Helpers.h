#ifndef HELPERS_H
#define HELPERS_H
#include <vector>
#include "ofApp.h"

class Neural_Net;

//Requires: two 2D vectors that can be multiplied
//Modifies: nothing
//Effects: Returns the matrix multiplication of the two matricies
std::vector<std::vector<double>> Matrix_Multiplication(std::vector<std::vector<double>> &one, std::vector<std::vector<double>> &two);

//Requires: double distance_reading
//Modifies: nothing
//Effects: Returns the number of pixels the object should get based on its distance
int Map_Distance_To_Pixels(double distance_reading);

//Requires: std::vector<std::vector<ofColor>> image
//Modifies: nothing
//Effects: Prints out the image to the standard output
void Print_Image(std::vector<std::vector<ofColor>> &image);

//Requires: std::vector<std::vector<ofColor>> image
//Modifies: nothing
//Effects: Returns an ofImage from the 2d image array
ofImage Create_Image(std::vector<std::vector<ofColor>> &image);

//Requires: std::vector<std::vector<ofColor>> image
//Modifies: nothing
//Effects: Decomposes the ofImage into a 30x30x3 array
std::vector<std::vector<std::vector<int>>> Decompose_Image(std::vector<std::vector<ofColor>> &image_in);

//Requires: std::vector<std::vector<ofColor>> image
//Modifies: nothing
//Effects: Returns the convolution matrix of the inputted image
std::vector<std::vector<double>> Convolute_Image(std::vector<std::vector<ofColor>> &image);

//Requires: std::vector<std::vector<double>> image
//Modifies: nothing
//Effects: Returns a vector that represents the pixel values of the image pooled along the columns
std::vector<double> Column_Pooling(std::vector<std::vector<double>> &image);

//Requires: std::vector<double> pooled_input
//Modifies: pooled_input
//Effects: Maps all the values in pooled_input into a value between (0,1)
void Map_Values(std::vector<double> &pooled_input);

//Requires: std::vector<std::vector<std::vector<double>>> x_layers, std::vector<std::vector<std::vector<double>>> y_layers
//Modifies: nothing
//Effect: Returns the "distance" between two neural networks to determine if they are the same
double Neural_Net_Distance(std::vector<std::vector<std::vector<double>>>& x_layers, std::vector<std::vector<std::vector<double>>>& y_layers);

//Requires: Neural_Net* a, Neural_Net* b
//Modifies: nothing
//Effect: Returns the factor by which the fitness of the individual should be decreased by fitness sharing
double Neural_Net_Sharing(Neural_Net* a, Neural_Net* b);

//Requires: std::vector<std::vector<ofColor>> camera_output, bool predator
//Modifies: camera_input
//Effects: Performs a color filter over the camera_output
void Color_Filter(std::vector<std::vector<ofColor>> &camera_output, bool predator);
#endif /* HELPERS_H */