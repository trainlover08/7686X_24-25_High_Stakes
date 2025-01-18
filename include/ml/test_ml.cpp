#include "file_io.hpp"
#include <ctime>
#include <random>

bool E_NEW_NET = 1;
unsigned int training_epochs = 1000;
unsigned int episodes = 1000;

NeuralNetwork test_neural_network;

void train_test_neural_network () {
    std::srand(std::time(0));

    AdamWOptimizer optimizer(0.0001, 0.9, 0.999, 0.01, 1e-4);

    test_neural_network.add_layer(Layer(1, 128, "relu", optimizer));
    test_neural_network.add_layer(Layer(128, 128, "relu", optimizer));
    test_neural_network.add_layer(Layer(128, 128, "relu", optimizer));
    test_neural_network.add_layer(Layer(128, 1, "linear", optimizer));

    if (E_NEW_NET) {
        test_neural_network.save("test_network_params.txt");
    }

    for (int i = 0; i <= training_epochs; ++i) {
        test_neural_network.load("test_network_params.txt");

        std::vector<double> logits;
        std::vector<double> labels;

        logits.resize(0);
        labels.resize(0);

        for (int j = 0; j < episodes; ++j) {
            // Get the labeled training data
                // this is test generated data
                // a representation of a distance sensor (obj distance ~= 4)
                double distance = ((std::rand() % 1000) / 250) + 3;
                double label = distance > 4? 0 : 1;
                distance += ((std::rand() % 200) - 100) / 1000.00f;
                
            auto logit = test_neural_network.forward({{distance}});

            logits.push_back(sigmoid_activation(logit[0][0]));
            labels.push_back(static_cast<double>(label));
        }

        std::vector<double> predictions;
        for (double logit : logits) {
            predictions.push_back(logit);
        }

        std::cout << logits[0] << ", " << labels[0] << std::endl;

        double loss = bce_loss(predictions, labels);

        //std::cout << loss << std::endl;

        double CLIP = 1.0;
        test_neural_network.backward({{loss}}, CLIP);
        test_neural_network.update_weights();
        test_neural_network.save("test_network_params.txt");
    }
}

int main () {
    train_test_neural_network();
    return 0;
}