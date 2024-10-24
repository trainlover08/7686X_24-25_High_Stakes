#include "NeuralNetwork.hpp"
#include <iostream>


void testForwardAndBackward() {
    // Create an Adam optimizer
    Adam optimizer(0.001, 0.9, 0.999);

    // Define a network with input size 3, and 2 hidden layers with 4 and 2 units respectively
    NeuralNetwork net(3, {4, 2}, optimizer);

    // Sample input for testing (1 batch with 3 inputs)
    Matrix input = {{0.5}, {0.1}, {0.9}};

    // Perform forward pass
    Matrix output = net.forward(input);
    std::cout << "Forward pass output:\n";
    printMatrix(output);

    // Create a fake loss gradient for testing backward pass
    Matrix loss_grad = {{0.1}, {0.2}};  // Assuming output size is 2

    // Perform backward pass
    auto [weight_grads, bias_grads] = net.backward(loss_grad);

    // Test weight update
    std::cout << "Updating weights...\n";
    net.update(weight_grads, bias_grads);

    std::cout << "Weights and biases updated successfully!\n";
}

int main() {
    try {
        testForwardAndBackward();
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
    }
    return 0;
}