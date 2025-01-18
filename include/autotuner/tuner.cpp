#include "ai/neural_network.cpp"
#include <vector>

AdamWOptimizer optimizer(0.01, 0.9, 0.999, 1e-8, 0.01);

//tune later
vector<double> pidParams = {1.0, 0.1, 0.01}; // Initial values for Kp, Ki, Kd
vector<double> m(pidParams.size(), 0.0);
vector<double> v(pidParams.size(), 0.0);

//fix later
vector<double> computeGradients(const vector<double>& pidParams, double performanceMetric) {
    // Implement gradient computation here based on your system dynamics
    vector<double> gradients(pidParams.size());
    // Example: Placeholder values
    gradients[0] = -0.1; // Gradient for Kp
    gradients[1] = -0.05; // Gradient for Ki
    gradients[2] = -0.02; // Gradient for Kd
    return gradients;
}

std::vector<double> update_gains () {
    

    // Compute gradients
    vector<double> gradients = computeGradients(pidParams, performanceMetric);

    // Update PID parameters using the optimizer
    optimizer.update(pidParams, m, v, gradients);

    return pidParams;
}