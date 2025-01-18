#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cassert>
#include <algorithm>
#include <deque>
#include <tuple>

#include <fstream>
#include <stdexcept>

using namespace std;

class ReplayBuffer {
private:
    struct Transition {
        std::vector<double> state;
        int action;
        double reward;
        std::vector<double> nextState;
        bool done;
    };

    std::deque<Transition> buffer;
    size_t capacity;
    std::mt19937 rng;

public:
    ReplayBuffer(size_t capacity) : capacity(capacity) {
        std::random_device rd;
        rng = std::mt19937(rd());
    }

    void storeTransition(const std::vector<double>& state, int action, double reward, const std::vector<double>& nextState, bool done) {
        if (buffer.size() >= capacity) {
            buffer.pop_front();
        }
        buffer.push_back({state, action, reward, nextState, done});
    }

    std::tuple<std::vector<std::vector<double>>, std::vector<int>, std::vector<double>, std::vector<std::vector<double>>, std::vector<bool>>
    sampleBatch(size_t batchSize) {
        std::vector<std::vector<double>> states(batchSize);
        std::vector<int> actions(batchSize);
        std::vector<double> rewards(batchSize);
        std::vector<std::vector<double>> nextStates(batchSize);
        std::vector<bool> dones(batchSize);

        std::uniform_int_distribution<size_t> dist(0, buffer.size() - 1);

        for (size_t i = 0; i < batchSize; ++i) {
            size_t idx = dist(rng);
            const Transition& transition = buffer[idx];

            states[i] = transition.state;
            actions[i] = transition.action;
            rewards[i] = transition.reward;
            nextStates[i] = transition.nextState;
            dones[i] = transition.done;
        }

        return {states, actions, rewards, nextStates, dones};
    }

    bool isReady(size_t batchSize) const {
        return buffer.size() >= batchSize;
    }
};

class AdamOptimizer {
public:
    double lr;
    double beta1;
    double beta2;
    double epsilon;
    int t;

    AdamOptimizer(double learning_rate, double beta1, double beta2, double epsilon)
        : lr(learning_rate), beta1(beta1), beta2(beta2), epsilon(epsilon), t(0) {}

    void update(vector<vector<double>>& weights, vector<vector<double>>& m, vector<vector<double>>& v, const vector<vector<double>>& grads) {
        t++;
        for (size_t i = 0; i < weights.size(); ++i) {
            for (size_t j = 0; j < weights[0].size(); ++j) {
                m[i][j] = beta1 * m[i][j] + (1 - beta1) * grads[i][j];
                v[i][j] = beta2 * v[i][j] + (1 - beta2) * grads[i][j] * grads[i][j];
                double m_hat = m[i][j] / (1 - pow(beta1, t));
                double v_hat = v[i][j] / (1 - pow(beta2, t));
                weights[i][j] -= lr * m_hat / (sqrt(v_hat) + epsilon);
            }
        }
    }

    void update(vector<double>& biases, vector<double>& m, vector<double>& v, const vector<double>& grads) {
        t++;
        for (size_t i = 0; i < biases.size(); ++i) {
            m[i] = beta1 * m[i] + (1 - beta1) * grads[i];
            v[i] = beta2 * v[i] + (1 - beta2) * grads[i] * grads[i];
            double m_hat = m[i] / (1 - pow(beta1, t));
            double v_hat = v[i] / (1 - pow(beta2, t));
            biases[i] -= lr * m_hat / (sqrt(v_hat) + epsilon);
        }
    }
};

class AdamWOptimizer {
public:
    double lr;         // Learning rate
    double beta1;      // Exponential decay rate for the first moment estimates
    double beta2;      // Exponential decay rate for the second moment estimates
    double epsilon;    // Small constant to prevent division by zero
    double weightDecay; // Weight decay coefficient (L2 regularization)
    int t;             // Time step

    AdamWOptimizer(double learning_rate, double beta1, double beta2, double epsilon, double weightDecay)
        : lr(learning_rate), beta1(beta1), beta2(beta2), epsilon(epsilon), weightDecay(weightDecay), t(0) {}

    void update(vector<vector<double>>& weights, vector<vector<double>>& m, vector<vector<double>>& v, const vector<vector<double>>& grads) {
        t++;
        for (size_t i = 0; i < weights.size(); ++i) {
            for (size_t j = 0; j < weights[0].size(); ++j) {
                // Update biased first moment estimate
                m[i][j] = beta1 * m[i][j] + (1 - beta1) * grads[i][j];
                
                // Update biased second raw moment estimate
                v[i][j] = beta2 * v[i][j] + (1 - beta2) * grads[i][j] * grads[i][j];
                
                // Compute bias-corrected first moment estimate
                double m_hat = m[i][j] / (1 - pow(beta1, t));
                
                // Compute bias-corrected second raw moment estimate
                double v_hat = v[i][j] / (1 - pow(beta2, t));
                
                // Apply weight decay
                weights[i][j] -= lr * weightDecay * weights[i][j];
                
                // Update weights with AdamW rule
                weights[i][j] -= lr * m_hat / (sqrt(v_hat) + epsilon);
            }
        }
    }

    void update(vector<double>& biases, vector<double>& m, vector<double>& v, const vector<double>& grads) {
        t++;
        for (size_t i = 0; i < biases.size(); ++i) {
            // Update biased first moment estimate
            m[i] = beta1 * m[i] + (1 - beta1) * grads[i];
            
            // Update biased second raw moment estimate
            v[i] = beta2 * v[i] + (1 - beta2) * grads[i] * grads[i];
            
            // Compute bias-corrected first moment estimate
            double m_hat = m[i] / (1 - pow(beta1, t));
            
            // Compute bias-corrected second raw moment estimate
            double v_hat = v[i] / (1 - pow(beta2, t));
            
            // Apply weight decay (biases typically don't have weight decay, but adding for completeness)
            biases[i] -= lr * weightDecay * biases[i];
            
            // Update biases with AdamW rule
            biases[i] -= lr * m_hat / (sqrt(v_hat) + epsilon);
        }
    }

    // Serialize optimizer state to a file
    void save(ofstream& outFile) const {
        outFile << lr << " " << beta1 << " " << beta2 << " " << epsilon << " " << weightDecay << " " << t << "\n";
    }

    // Deserialize optimizer state from a file
    void load(ifstream& inFile) {
        inFile >> lr >> beta1 >> beta2 >> epsilon >> weightDecay >> t;
    }
};


class Layer {
public:
    vector<vector<double>> weights;
    vector<double> biases;
    vector<vector<double>> grads_weights;
    vector<double> grads_biases;
    vector<vector<double>> m_weights;
    vector<vector<double>> v_weights;
    vector<double> m_biases;
    vector<double> v_biases;
    vector<vector<double>> cache_inputs;
    vector<vector<double>> cache_z;
    string activation;
    AdamWOptimizer optimizer;

    Layer(int input_dim, int output_dim, const string& activation, AdamWOptimizer optimizer)
        : optimizer(optimizer) {
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<> d(0, 0.01);
        normal_distribution<> dist(0, std::sqrt(2.0 / input_dim));

        this->activation = activation;
        weights.resize(input_dim, vector<double>(output_dim));
        grads_weights.resize(input_dim, vector<double>(output_dim, 0.0));
        m_weights.resize(input_dim, vector<double>(output_dim, 0.0));
        v_weights.resize(input_dim, vector<double>(output_dim, 0.0));
        biases.resize(output_dim, 0.0);
        grads_biases.resize(output_dim, 0.0);
        m_biases.resize(output_dim, 0.0);
        v_biases.resize(output_dim, 0.0);

        for (int i = 0; i < input_dim; ++i) {
            for (int j = 0; j < output_dim; ++j) {
                if(activation != "relu"){
                    weights[i][j] = d(gen);
                } else {
                    weights[i][j] = dist(gen);
                }
            }
        }
    }

    vector<vector<double>> forward(const vector<vector<double>>& inputs) {
        int batch_size = inputs.size();
        int output_dim = weights[0].size();

        vector<vector<double>> z(batch_size, vector<double>(output_dim));
        vector<vector<double>> a(batch_size, vector<double>(output_dim));

        for (int i = 0; i < batch_size; ++i) {
            for (int j = 0; j < output_dim; ++j) {
                for (int k = 0; k < inputs[0].size(); ++k) {
                    z[i][j] += inputs[i][k] * weights[k][j];
                }
                z[i][j] += biases[j];
                a[i][j] = (activation == "relu") ? max(0.0, z[i][j]) : z[i][j];
            }
        }

        cache_inputs = inputs;
        cache_z = z;
        return a;
    }

    vector<vector<double>> backward(const vector<vector<double>>& grad_output, double& grad_clip) {
        int batch_size = cache_inputs.size();
        int input_dim = cache_inputs[0].size();
        int output_dim = weights[0].size();

        vector<vector<double>> grad_inputs(batch_size, vector<double>(input_dim));

        for (int i = 0; i < batch_size; ++i) {
            for (int j = 0; j < output_dim; ++j) {
                double grad_z = (activation == "relu") ? ((cache_z[i][j] > 0) ? grad_output[i][j] : 0.0) : grad_output[i][j];

                if(grad_z < grad_clip) {
                    grad_z = grad_clip;
                } else if(grad_z > grad_clip) {
                    grad_z = -grad_clip;
                }

                grads_biases[j] += grad_z;
                for (int k = 0; k < input_dim; ++k) {
                    grads_weights[k][j] += cache_inputs[i][k] * grad_z;
                    grad_inputs[i][k] += weights[k][j] * grad_z;
                }
            }
        }

        return grad_inputs;
    }

    void update_weights() {
        optimizer.update(weights, m_weights, v_weights, grads_weights);
        optimizer.update(biases, m_biases, v_biases, grads_biases);
    }

    void reset_gradients() {
        for (auto& row : grads_weights) {
            fill(row.begin(), row.end(), 0.0);
        }
        fill(grads_biases.begin(), grads_biases.end(), 0.0);
    }

    // Serialize the layer's parameters to a file
    void save(ofstream& outFile) const {
        // Save weights
        for (const auto& row : weights) {
            for (double val : row) {
                outFile << val << " ";
            }
        }
        outFile << "\n";

        // Save biases
        for (double val : biases) {
            outFile << val << " ";
        }
        outFile << "\n";

        // Save optimizer states
        optimizer.save(outFile);
    }

    // Deserialize the layer's parameters from a file
    void load(ifstream& inFile) {
        // Load weights
        for (auto& row : weights) {
            for (double& val : row) {
                inFile >> val;
            }
        }

        // Load biases
        for (double& val : biases) {
            inFile >> val;
        }

        // Load optimizer states
        optimizer.load(inFile);
    }

private:
    double He_initialzation(double input_size) {
        // He initialization standard deviation
        double stddev = std::sqrt(2.0 / input_size);

        // Random number generator with normal distribution
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, stddev);

        return dist(gen);
    }
};

class NeuralNetwork {
public:
    vector<Layer> layers;

    void add_layer(const Layer& layer) {
        layers.push_back(layer);
    }

    vector<vector<double>> forward(const vector<vector<double>>& inputs) {
        vector<vector<double>> out = inputs;
        for (auto& layer : layers) {
            out = layer.forward(out);
        }
        return out;
    }

    void backward(const vector<vector<double>>& grad_output, double& grad_clip) {
        vector<vector<double>> grad = grad_output;
        for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
            grad = it->backward(grad, grad_clip);
        }
    }

    void update_weights() {
        for (auto& layer : layers) {
            layer.update_weights();
            layer.reset_gradients();
        }
    }

    void softUpdate(NeuralNetwork& targetNetwork, double tau) {
        assert(layers.size() == targetNetwork.layers.size());

        // Update each layer in the target network
        for (size_t i = 0; i < layers.size(); ++i) {
            Layer& localLayer = layers[i];
            Layer& targetLayer = targetNetwork.layers[i];

            // Update weights and biases for each layer
            for (size_t j = 0; j < localLayer.weights.size(); ++j) {
                for(size_t k = 0; k < localLayer.weights[j].size(); ++k) {
                    targetLayer.weights[j][k] = tau * localLayer.weights[j][k] + (1 - tau) * targetLayer.weights[j][k];
                }
            }

            for (size_t j = 0; j < localLayer.biases.size(); ++j) {
                targetLayer.biases[j] = tau * localLayer.biases[j] + (1 - tau) * targetLayer.biases[j];
            }
        }
    }

    // Serialize the entire neural network to a file
    void save(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error: Could not open file for writing: " << filename << endl;
            return;
        }

        // Save the number of layers
        outFile << layers.size() << "\n";

        // Save each layer
        for (const Layer& layer : layers) {
            outFile << layer.weights.size() << " " << layer.weights[0].size() << " " << layer.activation << "\n";
            layer.save(outFile);
        }

        outFile.close();
    }

    // Deserialize the entire neural network from a file
    void load(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error: Could not open file for reading: " << filename << endl;
            throw std::invalid_argument("Make sure that the file with that name exists. If it is the first generation, set the enumerated boolean 'E_NEW_NET' to 'true'");
            return;
        }

        // Load the number of layers
        size_t numLayers;
        inFile >> numLayers;

        layers.clear(); // Clear existing layers

        // Load each layer
        for (size_t i = 0; i < numLayers; ++i) {
            int input_dim, output_dim;
            string activation;
            inFile >> input_dim >> output_dim >> activation;

            AdamWOptimizer optimizer(0.001, 0.9, 0.999, 1e-8, 0.01); // Initialize optimizer with dummy values
            Layer layer(input_dim, output_dim, activation, optimizer);
            layer.load(inFile);
            layers.push_back(layer);
        }

        inFile.close();
    }
};

// Gradient ascent to update parameters
void gradientAscent(std::vector<double>& params, const std::vector<double>& gradients, double learningRate) {
    std::transform(params.begin(), params.end(), gradients.begin(), params.begin(), [learningRate](double p, double g) {
        return p + learningRate * g;
    });
}

// Compute the loss using negative log likelihood
double computeLoss(const std::vector<double>& logProbs, const std::vector<double>& advantages) {
    double loss = 0.0;
    for (size_t i = 0; i < logProbs.size(); ++i) {
        loss += logProbs[i] * advantages[i];
    }
    return loss;
}

// Compute the advantage using reward-to-go method
double computeAdvantage(const std::vector<double>& rewards, int t, double gamma) {
    double advantage = 0.0;
    double discount = 1.0;
    for (int i = t; i < rewards.size(); ++i) {
        advantage += discount * rewards[i];
        discount *= gamma;
    }
    return advantage;
}

// Function to safely compute log values with clipping
inline float safeLog(float x, float min_value = 1e-6f) {
    // Clip the input x to avoid log of zero or negative numbers
    float clipped_x = std::max(x, min_value);
    return std::log(clipped_x);
}

double sigmoid_activation (double z) {
    return 1.0 / (1.0 + exp(-z));
}

double bce_loss (const std::vector<double>& logits, const std::vector<double>& targets) {
    assert(logits.size() == targets.size());
    double loss = 0.0;
    int n = logits.size();
    for (int i = 0; i < n; ++i) {
        double pred = std::clamp(logits[i], 1e-10, 1.0 - 1e-10);
        int target = targets[i];
        loss += target * std::log(pred) + (1 - target) * std::log(1 - pred);
    }
    return -loss / n;
}

std::vector<double> softmax(const std::vector<double>& logits) {
    std::vector<double> probabilities(logits.size());
    double maxLogit = *std::max_element(logits.begin(), logits.end());

    // Exponentiate logits after subtracting maxLogit
    double sumExp = 0.0;
    for (size_t i = 0; i < logits.size(); ++i) {
        probabilities[i] = std::exp(logits[i] - maxLogit);
        sumExp += probabilities[i];
    }

    // Normalize probabilities
    for (double& prob : probabilities) {
        prob /= sumExp;
    }

    return probabilities;
}

double CCE_loss (const std::vector<double>& predictions, const std::vector<bool>& targets) {
    assert(predictions.size() == targets.size());

    double loss = 0.0;
    for (size_t i = 0; i < predictions.size(); ++i) {
        double pred = std::min(std::max(predictions[i], 1e-7), 1.0 - 1e-7);
        if (targets[i] == 1) {
            // Only sum the loss for the correct class
            loss += -std::log(pred);
        }
    }

    return loss;
}