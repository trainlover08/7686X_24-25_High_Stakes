#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cassert>
#include <algorithm>

#include "neural_network.cpp"

using namespace std;

vector<vector<double>> generate_data(int num_samples) {
    vector<vector<double>> data(num_samples, vector<double>(2));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10, 10);

    for (int i = 0; i < num_samples; ++i) {
        data[i][0] = dis(gen);
        data[i][1] = 3.0 * data[i][0] + 5.0; // y = 3x + 5 + noise
    }
    return data;
}

vector<vector<double>> extract_inputs(const vector<vector<double>>& data) {
    vector<vector<double>> inputs(data.size(), vector<double>(1));
    for (size_t i = 0; i < data.size(); ++i) {
        inputs[i][0] = data[i][0];
    }
    return inputs;
}

vector<vector<double>> extract_outputs(const vector<vector<double>>& data) {
    vector<vector<double>> outputs(data.size(), vector<double>(1));
    for (size_t i = 0; i < data.size(); ++i) {
        outputs[i][0] = data[i][1];
    }
    return outputs;
}

double mean_squared_error(const vector<vector<double>>& predicted, const vector<vector<double>>& actual) {
    double mse = 0.0;
    for (size_t i = 0; i < predicted.size(); ++i) {
        mse += pow(predicted[i][0] - actual[i][0], 2);
    }
    return mse / predicted.size();
}

void linear_regression_test() {
    AdamWOptimizer optimizer(0.05, 0.9, 0.999, 0.001, 1e-4);
    NeuralNetwork nn;
    nn.add_layer(Layer(1, 1, "linear", optimizer));

    // Generate data
    vector<vector<double>> data = generate_data(100);
    vector<vector<double>> inputs = extract_inputs(data);
    vector<vector<double>> targets = extract_outputs(data);

    double CLIP_THRESHOLD = 1.05;

    // Training
    for (int epoch = 0; epoch <= 70; ++epoch) {
        auto predictions = nn.forward(inputs);
        vector<vector<double>> errors(predictions.size(), vector<double>(1));
        for (size_t i = 0; i < predictions.size(); ++i) {
            errors[i][0] = -predictions[i][0] + targets[i][0];
        }
        nn.backward(errors, CLIP_THRESHOLD);
        nn.update_weights();

        if (epoch % 10 == 0) {
            double mse = mean_squared_error(predictions, targets);
            std::cout << "Epoch " << epoch << ", MSE: " << mse << endl;

            auto final_predictions = nn.forward(inputs);

            vector<vector<double>> final_input = {{4}};

            vector<vector<double>> intermediate_output = nn.forward(final_input);

            cout << "(4 * 3) + 5 = " << intermediate_output[0][0] << endl;
        }
    }
}

int main() {
    linear_regression_test();
    return 0;
}