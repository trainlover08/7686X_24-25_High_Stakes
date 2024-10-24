#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <cassert>

using namespace std;

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
    AdamOptimizer optimizer;

    Layer(int input_dim, int output_dim, const string& activation, AdamOptimizer optimizer)
        : optimizer(optimizer) {
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<> d(0, 0.01);

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
                weights[i][j] = d(gen);
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

    vector<vector<double>> backward(const vector<vector<double>>& grad_output) {
        int batch_size = cache_inputs.size();
        int input_dim = cache_inputs[0].size();
        int output_dim = weights[0].size();

        vector<vector<double>> grad_inputs(batch_size, vector<double>(input_dim));

        for (int i = 0; i < batch_size; ++i) {
            for (int j = 0; j < output_dim; ++j) {
                double grad_z = (activation == "relu") ? ((cache_z[i][j] > 0) ? grad_output[i][j] : 0.0) : grad_output[i][j];

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

    void backward(const vector<vector<double>>& grad_output) {
        vector<vector<double>> grad = grad_output;
        for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
            grad = it->backward(grad);
        }
    }

    void update_weights() {
        for (auto& layer : layers) {
            layer.update_weights();
            layer.reset_gradients();
        }
    }
};

void test_nn() {
    AdamOptimizer optimizer(0.001, 0.9, 0.999, 1e-8);
    NeuralNetwork nn;
    nn.add_layer(Layer(2, 3, "relu", optimizer));
    nn.add_layer(Layer(3, 2, "linear", optimizer));

    // Test forward pass
    vector<vector<double>> inputs = { {1, 2}, {3, 4} };
    auto outputs = nn.forward(inputs);
    assert(outputs[0].size() == 2 && "Forward pass output shape is incorrect.");

    // Test backward pass
    vector<vector<double>> grad_output = { {0.1, 0.2}, {0.3, 0.4} };
    nn.backward(grad_output);
    assert(!nn.layers[0].grads_weights.empty() && "Gradients not computed correctly.");

    // Test weight update
    nn.update_weights();
    assert(nn.layers[0].weights[0][0] != 0 && "Weights not updated correctly.");

    cout << "All tests passed." << endl;
}


class Simple1DEnv {
public:
    double position;
    double target_position;
    double action_bound;

    Simple1DEnv(double target, double bound) : position(0.0), target_position(target), action_bound(bound) {}

    vector<double> reset() {
        position = 0.0;
        return {position};
    }

    tuple<vector<double>, double, bool> step(double action) {
        action = max(-action_bound, min(action, action_bound)); // Clip action
        position += action;
        double reward = -abs(position - target_position); // Penalize distance from target
        bool done = abs(position - target_position) < 0.1;
        return {{position}, reward, done};
    }
};

// Define the Actor-Critic network
class ActorCritic {
public:
    NeuralNetwork actor;
    NeuralNetwork critic;

    ActorCritic(AdamOptimizer actor_optimizer, AdamOptimizer critic_optimizer) {
        // Actor network (policy)
        actor.add_layer(Layer(1, 16, "relu", actor_optimizer));
        actor.add_layer(Layer(16, 1, "linear", actor_optimizer));
        
        // Critic network (value)
        critic.add_layer(Layer(1, 16, "relu", critic_optimizer));
        critic.add_layer(Layer(16, 16, "relu", critic_optimizer));
        critic.add_layer(Layer(16, 1, "linear", critic_optimizer));
    }

    double get_action(const vector<double>& state) {
        auto action_values = actor.forward({state});
        return action_values[0][0];
    }

    double get_value(const vector<double>& state) {
        auto value = critic.forward({state});
        return value[0][0];
    }

    void train(vector<vector<double>> states, vector<double> actions, vector<double> rewards, vector<vector<double>> next_states, double gamma) {
        for (size_t i = 0; i < states.size(); ++i) {
            vector<double> state = states[i];
            vector<double> next_state = next_states[i];
            double reward = rewards[i];
            double action = actions[i];

            double value = get_value(state);
            double next_value = get_value(next_state);
            double td_error = reward + gamma * next_value - value;

            cout << "State: " << state[0] << ", Action: " << action << ", Reward: " << reward << ", Value: " << value << ", Next Value: " << next_value << ", TD Error: " << td_error << endl;

            // Critic gradients
            vector<vector<double>> critic_output_grad = {{td_error}};
            critic.backward(critic_output_grad);
            critic.update_weights();

            // Actor gradients (policy gradient)
            vector<vector<double>> actor_output_grad = {{-td_error * action}};
            actor.backward(actor_output_grad);
            actor.update_weights();
        }
    }
};

void actor_critic_test() {
    double gamma = 0.99;
    AdamOptimizer actor_optimizer(0.100, 0.9, 0.999, 1e-3);
    AdamOptimizer critic_optimizer(0.001, 0.9, 0.999, 1e-8);

    ActorCritic ac(actor_optimizer, critic_optimizer);
    Simple1DEnv env(10.0, 1.0);

    int num_episodes = 500;
    int max_steps_per_episode = 200;

    for (int episode = 0; episode < num_episodes; ++episode) {
        vector<double> state = env.reset();
        double episode_reward = 0.0;

        for (int step = 0; step < max_steps_per_episode; ++step) {
            double action = ac.get_action(state);
            auto [next_state, reward, done] = env.step(action);

            cout << "Episode " << episode << ", Step " << step << ", State: " << state[0] << ", Action: " << action << ", Reward: " << reward << ", Next State: " << next_state[0] << endl;

            ac.train({state}, {action}, {reward}, {next_state}, gamma);

            state = next_state;
            episode_reward += reward;

            if (done) break;
        }

        if (episode % 50 == 0) {
            cout << "Episode " << episode << ", Reward: " << episode_reward << endl;
        }
    }
}

int main() {
    actor_critic_test();
    return 0;
}