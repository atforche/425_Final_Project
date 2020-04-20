#ifndef HELPERS_H
#define HELPERS_H
#include <vector>
#include "ofApp.h"

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
void Print_Image(std::vector<std::vector<ofColor>> image);

//Requires: std::vector<std::vector<ofColor>> image
//Modifies: nothing
//Effects: Returns an ofImage from the 2d image array
ofImage Create_Image(std::vector<std::vector<ofColor>> image);

//Requires: std::vector<std::vector<ofColor>> image
//Modifies: nothing
//Effects: Decomposes the ofImage into a 30x30x3 array
std::vector<std::vector<std::vector<int>>> Decompose_Image(std::vector<std::vector<ofColor>> image_in);


#endif /* HELPERS_H */