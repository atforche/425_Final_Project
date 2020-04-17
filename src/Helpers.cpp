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