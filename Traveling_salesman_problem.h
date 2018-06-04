#ifndef CA_UWATERLOO_ALUMNI_DWHARDER_TRAVELING_SALESMAN_PROBLEM
#define CA_UWATERLOO_ALUMNI_DWHARDER_TRAVELING_SALESMAN_PROBLEM

#include <algorithm>
#include <limits>
#include <cstdlib>
#include <map>
#include <ctime>

// We are assuming that we are finding the minimum path:
//  - starting at vertex v_0
//  - passing through the points in S
//  - ending at vertex v_k

namespace Traveling_salesman_problem {
	// A top-down implementation without memoization
	//  - the run time will be O(n!)

	float tsp_top_down( unsigned int S, int k, int n, float **adjacency_matrix ) {
        	if ( S == 0 ) {
                	return adjacency_matrix[0][k];
        	}

        	float minimum = std::numeric_limits<float>::infinity();

        	for ( unsigned int j = 1, mask = 2; j < n; ++j, mask <<= 1 ) {
                	if ( S & mask ) {
                        	minimum = std::min(
                                	minimum,
                                	tsp_top_down( S & ~mask, j, n, adjacency_matrix ) + adjacency_matrix[j][k]
                        	);
                	}
        	}

        	return minimum;
	}

	// A top-down implementation with memoization
	//  - the run time will be O(n^2*2^n)
	//  - the memory requirements will be O(n*2^n)

	float tsp_top_down_memoization( unsigned int S, int k, int n, float **adjacency_matrix, bool clear = true ) {
        	static std::map< std::pair< unsigned int, int >, float > memo;

        	if ( S == 0 ) {
                	return adjacency_matrix[0][k];
        	}

        	if ( clear ) {
                	memo.clear();
        	}

        	if ( memo.find( std::pair<unsigned int, int>( S, k ) ) == memo.end() ) {
                	float minimum = std::numeric_limits<float>::infinity();

                	for ( unsigned int j = 1, mask = 2; j < n; ++j, mask <<= 1 ) {
                        	if ( S & mask ) {
                                	minimum = std::min( minimum, tsp_top_down_memoization(
						S & ~mask, j, n, adjacency_matrix, false
					) + adjacency_matrix[j][k] );
                        	}
                	}

                	memo[std::pair<unsigned int, int>(S, k)] = minimum;
        	}

        	return memo[std::pair<unsigned int, int>(S, k)];
	}

	// A bottom-up implementation
	//  - the run time will be O(n^2*2^n)
	//  - the memory requirements will be O(n*2^n) and this must be an n x 2^n 2-d array
	// The memory requirements could be reduced slightly by keeping only sets of size
	// |S| = m and |S| = m - 1.

	float tsp_bottom_up( unsigned int S, int k, int n, float **adjacency_matrix ) {
		float array[1 << n][n];

        	for ( int k = 0; k < n; ++k ) {
                	array[0][k] = adjacency_matrix[0][k];
        	}

        	for ( int s = 2; s <= S; s += 2 ) {
                	for ( int k = 0; k < n; ++k ) {
                        	float minimum = std::numeric_limits<float>::infinity();

                        	for ( unsigned int j = 1, mask = 2; j < n; ++j, mask <<= 1 ) {
                                	if ( s & mask ) {
                                        	minimum = std::min(
                                                	minimum,
                                                	array[s & ~mask][j] + adjacency_matrix[j][k]
                                        	);
                                	}
                        	}

                        	array[s][k] = minimum;
                	}
        	}

        	float minimum = std::numeric_limits<float>::infinity();

		// Find the shortest distance by finding the shortest of
		// each of the cycles with a second-last point being 

        	for ( unsigned int j = 1, mask = 2; j < n; ++j, mask <<= 1 ) {
                	minimum = std::min(
                        	minimum,
                        	array[S & ~mask][j] + adjacency_matrix[j][0]
                	);
        	}

        	return minimum;
	}
}

#endif