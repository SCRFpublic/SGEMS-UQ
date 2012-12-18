#pragma once

/* Multidimensional scaling

Alexander Chia
alexchia@stanford.edu
*/

#include "common.h"
#include <eigen3/Eigen/Dense>
#include <iostream>

using namespace Eigen;
using namespace std;
using Eigen::MatrixXf;

class ITEM_MODEL_DECL mds {
public:
	mds(void) {};
	~mds(void) {};

	// n * n upper triangular distance matrix, n, output data array (of vectors), dimension out
	bool do_mds(MatrixXf& distance, int numElts, float* output_data,  int out_dimension);

};

// on a upper triangular distance matrix
bool mds::do_mds(MatrixXf& distance, int numElts, float* output_data, int out_dimension) {
	int n = numElts;
	int i, j;

	// debug
	cout << "Distance matrix:" << endl;
	cout << distance << endl;

	// assert distance matrix dimensions
	if (distance.cols() != distance.rows()) {
		cout << "Error: Distance matrix should be square and symmetric." << endl;
		return false;
	}

	// output dimensions shouldn't be more than the rank of the matrix
	if (distance.rows() < out_dimension) {
		cout << "Error: Output dimension greater than distance matrix dimension." << endl;
		return false;
	}

	// generate squared distances
	MatrixXf squared_distance(n, n);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (i < j)
				squared_distance(i,j) = distance(i,j) * distance(i,j);
			else if (i == j)
				squared_distance(i,j) = 0;
			else
				squared_distance(i,j) = squared_distance(j, i);
		}
	}

	/** centering */

	// calculate column and row sums
	VectorXf col_sum(n);
	VectorXf row_sum(n);

	float sum = 0.0f;
	for (i = 0; i < n; i++) {
		sum = 0.0f;
		for (j = 0; j < n; j++) {
			sum += squared_distance(i, j);
		}
		row_sum(i) = sum;
	}
	for (i = 0; i < n; i++) {
		sum = 0.0f;
		for (j = 0; j < n; j++) {
			sum += squared_distance(j, i);
		}
		col_sum(i) = sum;
	}
	
	float total_sum = 0.0f;
	for (i = 0; i < n; i++) {
		total_sum += row_sum(i);
	}

	// now center
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			squared_distance(i,j) = (squared_distance(i,j)
								    - row_sum(i) / (float)n
								    - col_sum(j) / (float)n
									+ total_sum / (float)n / (float)n) / -2.0f;
		}
	}

	/** Do multidimensional scaling */

	// SVD
	JacobiSVD<MatrixXf> svd(squared_distance, ComputeThinU);
	// get first p dimensions of U
	MatrixXf leftU = svd.matrixU().leftCols(out_dimension);

	VectorXf sv = svd.singularValues();
	for (i = 0; i < n; i++) {
		for (j = 0; j < out_dimension; j++) {
			if (sv(j) > 0.0f) {
				output_data[i * out_dimension + j] = leftU(i,j) * sqrt(sv(j));
			} else {
				// error : negative eigenvalue
				output_data[i * out_dimension + j] = 0.0f;
			}
		}
	}

	return true;
}
