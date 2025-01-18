#include "file_io.hpp"
#include <ctime>

// Ordering within the vector for sort
    // Distance
    // Object Brightness
    // Object Blue value
    // Object Red value
    // Object Green value
    // Label

bool E_NEW_NET = 0;
bool E_READ_FROM_FILE = 0;
unsigned int training_epochs = 100;
unsigned int episodes = 10;
unsigned int training_buff_size = 100;

NeuralNetwork nn;
File_IO file_io;

void train_nn () {
    std::srand(std::time(0));

    AdamWOptimizer optimizer(0.0001, 0.9, 0.999, 0.01, 1e-4);

    nn.add_layer(Layer(3, 128, "relu", optimizer));
    nn.add_layer(Layer(128, 128, "relu", optimizer));
    nn.add_layer(Layer(128, 128, "relu", optimizer));
    nn.add_layer(Layer(128, 128, "relu", optimizer));
    nn.add_layer(Layer(128, 128, "relu", optimizer));
    nn.add_layer(Layer(128, 3, "linear", optimizer));

    if (E_NEW_NET) {
        nn.save("test_network_params.txt");
    }

    double rate = 0;

    for (int i = 0; i <= training_epochs; ++i) {
        nn.load("test_network_params.txt");
        std::vector<std::vector<double>> buffer(6);
        
        for (int g = 0; g < buffer.size(); ++g) {
            buffer[g].resize(training_buff_size);
        }

        if (E_READ_FROM_FILE) {
            buffer = file_io.read("training_data.txt");
        } else {
            for (int k = 0; k < training_buff_size; ++k) {
                buffer[0][k] = (std::rand() % 1000) / 100.0f;
                buffer[1][k] = (std::rand() % 10) / 10.0f;
                buffer[2][k] = std::rand() % 256;
                buffer[3][k] = std::rand() % 256;
                buffer[4][k] = std::rand() % 256;
                buffer[5][k] = buffer[0][k] < 5 ? 0 : buffer[3][k] > buffer[2][k] ? 1 : 2;
                    // 0 - no object
                    // 1 - red
                    // 2 - blue
            }
        }

        double loss = 0.0;

        for (int j = 0; j < episodes; ++j) {
            // Pull random sample
            unsigned int index = std::rand() % buffer[0].size();
            double distance = buffer[0][index];
            double brightness = buffer[1][index];
            double blue = buffer[2][index];
            double red = buffer[3][index];
            double green = buffer[4][index];
            double label = buffer[5][index];
            // Prepare to a 0 - 1 state
            distance = std::sqrt(distance) / 3.0f;
            // brightness is already correctly scaled
            blue = blue / 256.0f;
            red = red / 256.0f;
            green = green / 256.0f;

            std::vector<double> input = {distance, blue, red};
            std::vector<std::vector<double>> predictions = nn.forward({input});

            // Set a binary encoded target vector from the label
            std::vector<bool> target = {0, 0, 0};
            // A clever way to find which value is true
            target[buffer[5][index]] = 1;

            std::vector<double> probs = softmax(predictions[0]);

            loss += CCE_loss(probs, target);

            if (i > training_epochs - 50) {
                rate += target[*std::max_element(probs.begin(), probs.end())];
            }
        }

        loss /= training_epochs;



        //std::cout << loss << std::endl;

        double CLIP = 0.1;
        nn.backward({{loss}}, CLIP);
        nn.update_weights();
        nn.save("test_network_params.txt");
    }

    std::cout << rate / (50 * episodes) << std::endl;
}

int main () {
    train_nn();
    std::vector<std::vector<double>> output = nn.forward({{2.0f, 200.0f, 20.0f}});
    std::vector<double> out = softmax(output[0]);
    for (int f = 0; f < out.size(); f++) {
        std::cout << out[f] << ", ";
    }
    std::cout << std::endl;
    return 0;
}