#include "Brain.h"
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
	if (distance > 720) {
		return 16;
	}
	else if (distance > 640) {
		return 20;
	}
	else if (distance > 580) {
		return 24;
	}
	else if (distance > 420) {
		return 28;
	}
	else if (distance > 360) {
		return 32;
	}
	else if (distance > 300) {
		return 36;
	}
	else if (distance > 240) {
		return 40;
	}
	else if (distance > 180) {
		return 44;
	}
	else if (distance > 120) {
		return 48;
	}
	else if (distance > 60) {
		return 52;
	}
	else {
		return 56;
	}
}
//--------------------------------------------------------------------------------------

void Print_Image(std::vector<std::vector<ofColor>> &image) {
	ofColor sky_color(0, 229, 255);
	ofColor ground_color(255, 204, 153);

	for (int i = 0; i < image.size(); ++i) {
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

ofImage Create_Image(std::vector<std::vector<ofColor>> &image) {
	ofPixels picture;
	picture.allocate(image.size(), image[0].size(), OF_IMAGE_COLOR);

	for (int i = 0; i < image.size(); ++i) {
		for (int j = 0; j < image[0].size(); ++j) {
			picture.setColor(i, j, image[i][j]);
		}
	}

	return ofImage(picture);
}

//--------------------------------------------------------------------------------------

std::vector<std::vector<std::vector<int>>> Decompose_Image(std::vector<std::vector<ofColor>> &image_in) {
	//Initialize a 30x30x3 array
	std::vector<std::vector<std::vector<int>>> image;
	image.resize(image_in.size());
	for (size_t i = 0; i < image_in[0].size(); ++i) {
		image[i].resize(image_in[0].size());
		for (size_t j = 0; j < image[i].size(); ++j) {
			image[i][j].resize(3);
		}
	}

	for (int i = 0; i < image.size(); ++i) {
		for (int j = 0; j < image[0].size(); ++j) {
			image[i][j][0] = image_in[j][i].r;
			image[i][j][1] = image_in[j][i].g;
			image[i][j][2] = image_in[j][i].b;
		}
	}

	return image;
}

//--------------------------------------------------------------------------------------

std::vector<std::vector<double>> Convolute_Image(std::vector<std::vector<ofColor>> &image) {
	int filter_size = 3;
	//Convolution filters used to identify square/rectangular regions in the image
	//Filters are left vertical, right vertical, upper horizontal, lower horizontal, and the four corners
	std::vector<std::vector<std::vector<int>>> con_filters = {
		{{-2,1,1},{-2,1,1},{-2,1,1}},
		{{1,1,-2},{1,1,-2},{1,1,-2}},
		{{-2,-2,-2},{1,1,1},{1,1,1}},
		{{1,1,1},{1,1,1},{-2,-2,-2}},
		{{-1,-1,-1},{-1,2,2},{-1,2,2}},
		{{-1,-1,-1},{2,2,-1},{2,2,-1}},
		{{-1,2,2},{-1,2,2},{-1,-1,-1}},
		{{2,2,-1},{2,2,-1},{-1,-1,-1}}
	};

	size_t cols = image.size();
	size_t rows = image[0].size();

	//Initialize the convolution matrix
	std::vector<std::vector<double>> convolution;
	convolution.resize(cols-2);
	for (size_t i = 0; i < convolution.size(); ++i) {
		convolution[i].resize(rows-2);
	}

	for (size_t item = 0; item < con_filters.size(); ++item) {
		for (size_t i = 1; i < cols - filter_size + 2; ++i) {
			for (size_t j = 1; j < rows - filter_size + 2; ++j) {
				double sum = 0;
				//Now apply the filter over the 3x3 region surrounding the current pixel
				for (size_t x = 0; x < filter_size; ++x) {
					for (size_t y = 0; y < filter_size; ++y) {
						sum += int(image[i + x - 1][j + y - 1].r) * con_filters[item][x][y];
						sum += int(image[i + x - 1][j + y - 1].g) * con_filters[item][x][y];
						sum += int(image[i + x - 1][j + y - 1].b) * con_filters[item][x][y];
					}
				}
				convolution[j - 1][i - 1] += (sum / 255);
			}
		}
	}

	return convolution;
}

//--------------------------------------------------------------------------------------

std::vector<double> Column_Pooling(std::vector<std::vector<double>> &image) {
	std::vector<double> output;
	output.resize(image.size());

	for (size_t i = 0; i < image[0].size(); ++i) {
		double sum = 0;
		for (size_t j = 0; j < image.size(); ++j) {
			sum += image[j][i];
		}
		output[i] = sum;
	}

	return output;
}

//--------------------------------------------------------------------------------------

void Map_Values(std::vector<double> &pooled_input) {
	double max_a = *max_element(pooled_input.begin(), pooled_input.end());
	double min_a = *min_element(pooled_input.begin(), pooled_input.end());
	//https://rosettacode.org/wiki/Map_range
	for (size_t i = 0; i < pooled_input.size(); ++i) {
		pooled_input[i] = (pooled_input[i] - min_a) / (max_a - min_a);
	}
}

//--------------------------------------------------------------------------------------

double Neural_Net_Distance(std::vector<std::vector<std::vector<double>>>& x_layers, std::vector<std::vector<std::vector<double>>>& y_layers) {

	//All of the Neural Nets in this simulation has three layers
	std::vector<std::vector<double>> x_layer_one = x_layers[0];
	std::vector<std::vector<double>> y_layer_one = y_layers[0];

	std::vector<std::vector<double>> x_layer_two = x_layers[1];
	std::vector<std::vector<double>> y_layer_two = y_layers[1];

	std::vector<std::vector<double>> x_layer_three = x_layers[2];
	std::vector<std::vector<double>> y_layer_three = y_layers[2];

	double sum = 0;

	for (size_t i = 0; i < x_layer_one.size(); ++i) {
		for (size_t j = 0; j < x_layer_one[0].size(); ++j) {
			double difference = abs(x_layer_one[i][j] - y_layer_one[i][j]);
			sum += difference;
		}
	}

	for (size_t i = 0; i < x_layer_two.size(); ++i) {
		for (size_t j = 0; j < x_layer_two[0].size(); ++j) {
			double difference = abs(x_layer_two[i][j] - y_layer_two[i][j]);
			sum += difference;
		}
	}

	for (size_t i = 0; i < x_layer_three.size(); ++i) {
		for (size_t j = 0; j < x_layer_three[0].size(); ++j) {
			double difference = abs(x_layer_three[i][j] - y_layer_three[i][j]);
			sum += difference;
		}
	}

	return sum;
}

//--------------------------------------------------------------------------------------

double Neural_Net_Sharing(Neural_Net* a, Neural_Net* b) {
	double threshold = 3;
	double distance = Neural_Net_Distance(a->Get_Layers(), b->Get_Layers());
	return std::max(0.0, 1 - (distance / threshold));
}

//--------------------------------------------------------------------------------------

