#include "Helpers.h"
#include <cassert>

std::vector<std::vector<double>> Matrix_Multiplication(std::vector<std::vector<double>> &one, std::vector<std::vector<double>> &two) {

	assert(one[0].size() == two.size());

	int output_rows = one.size();
	int output_cols = two[0].size();

	std::vector<std::vector<double>> output;
	output.resize(output_rows);
	for (int i = 0; i < output_rows; ++i) {
		output[i].resize(output_cols);
	}

	//Loop through the rows in the first array
	for (int i = 0; i < one.size(); ++i) {

		//Perform the dot product of this row with every column in two
		for (int j = 0; j < two[0].size(); ++j) {

			double sum = 0;

			//Multiple each corresponding element in the row of one and the column of two together
			//output[i][j] is equal to the sum of the products
			for (int k = 0; k < one[0].size(); ++k) {

				sum += one[i][k] * two[k][j];
			}

			output[i][j] = sum;

		}
	}

	return output;
}

//--------------------------------------------------------------------------------------

int Map_Distance_To_Pixels(double distance_reading) {
	int distance = int(distance_reading);
	if (distance > 400) {
		return 0;
	}
	else if (distance > 360) {
		return 2;
	}
	else if (distance > 320) {
		return 4;
	}
	else if (distance > 280) {
		return 6;
	}
	else if (distance > 240) {
		return 8;
	}
	else if (distance > 200) {
		return 10;
	}
	else if (distance > 160) {
		return 12;
	}
	else if (distance > 120) {
		return 14;
	}
	else if (distance > 80) {
		return 16;
	}
	else if (distance > 40) {
		return 18;
	}
	else if (distance > 20) {
		return 20;
	}
	else {
		return 24;
	}
}
//--------------------------------------------------------------------------------------

void Print_Image(std::vector<std::vector<ofColor>> image) {
	ofColor sky_color(0, 229, 255);
	ofColor ground_color(255, 204, 153);

	for (int i = image.size() - 1; i >= 0; --i) {
		for (int j = 0; j < image[0].size(); ++j) {
			if (image[j][i] == sky_color) {
				std::cout << "s ";
			}
			else if (image[j][i] == ground_color) {
				std::cout << "g ";
			}
			else if (image[j][i] == ofColor(255, 0, 0)) {
				std::cout << "E ";
			}
			else if (image[j][i] == ofColor(0, 255, 0)) {
				std::cout << "W ";
			}
			else if (image[j][i] == ofColor(0, 0, 255)) {
				std::cout << "P ";
			}
		}
		std::cout << '\n';
	}
}
//--------------------------------------------------------------------------------------

ofImage Create_Image(std::vector<std::vector<ofColor>> image) {
	ofPixels picture;
	picture.allocate(image.size(), image[0].size(), OF_IMAGE_COLOR);

	for (int i = image.size() - 1; i >= 0; --i) {
		for (int j = 0; j < image[0].size(); ++j) {
			picture.setColor(i, j, image[j][i]);
		}
	}


	return ofImage(picture);
}