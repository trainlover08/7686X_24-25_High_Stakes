#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include "Optimizers.hpp"
#include "Initialization.hpp"
#include <vector>
#include <stdexcept>
#include <cmath>

using Matrix = std::vector<std::vector<double>>;

class NeuralNetwork {
private:
    std::vector<Matrix> weights;       // Weights of the layers
    std::vector<Matrix> biases;        // Biases of the layers
    std::vector<Matrix> activations;   // Activations per layer
    Adam optimizer;                    // Optimizer (Adam for now, can swap later)
    
    // Initialize weights and biases for each layer
    void initializeWeightsAndBiases(int input_size, const std::vector<int>& layer_sizes, bool useHeInitialization = false) {
        int prev_size = input_size;

        for (int size : layer_sizes) {
            // Initialize weights based on the specified initialization method
            if (useHeInitialization) {
                weights.push_back(heInitialization(prev_size, size));  // Use He initialization
            } else {
                weights.push_back(normalInitialization(prev_size, size));  // Use normal initialization
            }

            // Initialize biases to zeros
            biases.push_back(normalInitialization(size, 1, 0.0, 0.0)); // Optionally can also have a separate initialization method for biases if needed
            prev_size = size;
        }
    }

public:
    // Constructor: Initialize network with input size and layers
    NeuralNetwork(int input_size, const std::vector<int>& layer_sizes, Adam optimizer_)
        : optimizer(optimizer_) {
        initializeWeightsAndBiases(input_size, layer_sizes);
    }

    // Forward pass
    Matrix forward(const Matrix& input) {
        Matrix current_activation = input;
        activations.push_back(current_activation);  // Save input as first activation
        
        for (size_t i = 0; i < weights.size(); ++i) {
            std::cout << "not my fault" <<'\n';
            current_activation = add(multiply(weights[i], current_activation), biases[i]); // z = Wx + b
            std::cout << "made it this far" << '\n';
            current_activation = applyActivation(current_activation);                    // Apply activation (ReLU or similar)
            activations.push_back(current_activation);                                   // Save activations per layer
        }

        return current_activation;
    }

    // Apply an activation function (e.g., ReLU, sigmoid, etc.)
    Matrix applyActivation(const Matrix& z) {
        Matrix activated = z;
        for (size_t i = 0; i < z.size(); ++i) {
            for (size_t j = 0; j < z[0].size(); ++j) {
                activated[i][j] = std::max(0.0, z[i][j]);  // ReLU as default
            }
        }
        return activated;
    }

    // Backward pass for calculating gradients
    std::pair<std::vector<Matrix>, std::vector<Matrix>> backward(const Matrix& loss_grad) {
        std::vector<Matrix> weight_gradients(weights.size());
        std::vector<Matrix> bias_gradients(biases.size());

        Matrix delta = loss_grad;  // Assume the loss gradient is calculated elsewhere
        for (int i = weights.size() - 1; i >= 0; --i) {
            // Calculate gradients w.r.t. weights and biases
            weight_gradients[i] = multiply(delta, transpose(activations[i]));  // dW = delta * a^(l-1)^T
            bias_gradients[i] = delta;                                       // db = delta
            
            if (i > 0) {
                // Calculate delta for the next layer
                delta = multiply(transpose(weights[i]), delta);
            }
        }

        return { weight_gradients, bias_gradients };
    }

    // Update the network's parameters using Adam optimizer
    void update(const std::vector<Matrix>& weight_grads, const std::vector<Matrix>& bias_grads) {
        for (size_t i = 0; i < weights.size(); ++i) {
            weights[i] = optimizer.update(weights[i], weight_grads[i]);
            biases[i] = optimizer.update(biases[i], bias_grads[i]);
        }
    }
};

#endif  // NEURAL_NETWORK_HPP