#include <iostream>
#include "Traveling_salesman_problem.h"

// The argument indicates how large the graph should be

using namespace Traveling_salesman_problem;

int main( int argc, char* argv[] ) {
	int n = atoi( argv[1] );

	float **adjacency_matrix = new float *[n];
	adjacency_matrix[0] = new float[n*n];

	for ( int i = 1; i < n; ++i ) {
		adjacency_matrix[i] = adjacency_matrix[i - 1] + n;
	}

	for ( int j = 0; j < n; ++j ) {
		adjacency_matrix[j][j] = 0.0;

		for ( int k = 0; k < j; ++k ) {
			adjacency_matrix[j][k] = 1.0 + drand48();
			adjacency_matrix[k][j] = adjacency_matrix[j][k];
		}
	}

	// Starting with vertex v_0

	std::cout << tsp_top_down( (1 << n) - 2, 0, n, adjacency_matrix )             << std::endl;
	std::cout << tsp_top_down_memoization( (1 << n) - 2, 0, n, adjacency_matrix ) << std::endl;
	std::cout << tsp_bottom_up( (1 << n) - 2, 0, n, adjacency_matrix )            << std::endl;

	return 0;
}