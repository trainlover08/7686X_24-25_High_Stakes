#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include <vector>
#include <random>
#include <cmath>

// Type alias for matrix
using Matrix = std::vector<std::vector<double>>;

// Function to generate random numbers from a normal distribution
double randomNormal(double mean, double stddev) {
    static std::mt19937 generator{std::random_device{}()};
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(generator);
}

// Function for normal (Gaussian) initialization
Matrix normalInitialization(int rows, int cols, double mean = 0.0, double stddev = 1.0) {
    Matrix weights(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            weights[i][j] = randomNormal(mean, stddev);
        }
    }
    return weights;
}

// Function for He initialization
Matrix heInitialization(int rows, int cols) {
    double stddev = std::sqrt(2.0 / rows); // Standard deviation based on number of input units
    return normalInitialization(rows, cols, 0.0, stddev);
}

#endif // INITIALIZATION_HPP