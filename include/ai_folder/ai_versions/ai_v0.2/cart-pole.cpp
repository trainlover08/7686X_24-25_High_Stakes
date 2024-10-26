#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <cassert>
#include <cstdlib>

// Placeholder for the neural network and cart-pole environment
#include "neural_network.cpp"
#include "cart-pole-env.cpp"

bool E_NEW_NET = true;


// Function to sample an action based on action probabilities
int sampleAction(const std::vector<double>& actionProbs) {
    assert(actionProbs.size() == 1);  // For CartPole, we have two actions (left, right)
    
    double p = static_cast<double>(rand()) / RAND_MAX;
    return (p < actionProbs[0]) ? 0 : 1;
}

// Main training loop for policy gradient
void trainCartPolePolicyGradient(NeuralNetwork& actor, NeuralNetwork& critic, LinearRegression& env, int numEpisodes, double gamma, double learningRate, double GRADIENT_CLIP_THRESHOLD, double weight_decay) {
    AdamWOptimizer actorOptimizer(learningRate, 0.9, 0.999, 0.01, weight_decay);
    AdamWOptimizer criticOptimizer(learningRate, 0.9, 0.999, 0.01, weight_decay);

    actor.add_layer(Layer(2, 128, "relu", actorOptimizer));
    actor.add_layer(Layer(128, 128, "relu", actorOptimizer));
    actor.add_layer(Layer(128, 1, "linear", actorOptimizer));  // Output probabilities for actions

    critic.add_layer(Layer(2, 64, "relu", criticOptimizer));
    critic.add_layer(Layer(64, 64, "relu", criticOptimizer));
    critic.add_layer(Layer(64, 1, "linear", criticOptimizer));  // Single output for state value

    if(E_NEW_NET == 1) {
        // Save neural network to file
        actor.save("actor_network_params.txt");
        critic.save("critic_network_params.txt");
    }

    for (int episode = 0; episode <= numEpisodes; ++episode) {
        // Load neural network from file
        NeuralNetwork actorLoadedNN;
        NeuralNetwork criticLoadedNN;
        actorLoadedNN.load("actor_network_params.txt");
        criticLoadedNN.load("critic_network_params.txt");

        actor = actorLoadedNN;
        critic = criticLoadedNN;

        std::vector<std::vector<double>> states;
        std::vector<int> actions;
        std::vector<double> rewards, logProbs, values;

        env.reset();
        while (!env.isDone()) {
            std::vector<double> state = env.getState();
            states.push_back(state);

            // Get action probabilities from the actor network
            std::vector<std::vector<double>> actionProbs = actor.forward({state});

            // Sample an action based on the probabilities
            double action = actionProbs[0][0];  // Use the actionProbs for action sampling
            actions.push_back(action);

            // std::cout << "Action: " << action << '\n';
            
            // Log probability of the action
            logProbs.push_back(std::log(std::max(action, 1e-8)));

            // Get the value estimate from the critic network
            std::vector<std::vector<double>> valueEstimates = critic.forward({state});
            values.push_back(valueEstimates[0][0]);

            // Take the action in the environment
            env.step(action);

            // Store the reward
            rewards.push_back(env.getReward());
        }

        // Compute the advantages using the critic network
        std::vector<double> advantages;
        double reward = 0;

        for (int t = 0; t < rewards.size(); ++t) {
            double td_target = rewards[t] + (t < rewards.size() - 1 ? gamma * values[t + 1] : 0.0);
            advantages.push_back(td_target - values[t]);
            reward += rewards[t];
        }

        // Compute the policy (actor) loss
        double actorLoss = computeLoss(logProbs, advantages);

        if(episode % 100 == 0) {
            std::cout << "Episode " << episode << ", Actor Loss: " << actorLoss << ", Reward: " << reward << std::endl;
        }

        // Compute the critic loss (mean squared error)
        double criticLoss = 0.0;
        for (size_t i = 0; i < rewards.size(); ++i) {
            double td_target = rewards[i] + (i < rewards.size() - 1 ? gamma * values[i + 1] : 0.0);
            criticLoss += pow(td_target - values[i], 2);
        }
        criticLoss /= rewards.size();

        // Backpropagate and update actor network
        actor.backward({{actorLoss}}, GRADIENT_CLIP_THRESHOLD);
        actor.update_weights();

        // Backpropagate and update critic network
        critic.backward({{criticLoss}}, GRADIENT_CLIP_THRESHOLD);
        critic.update_weights();

        // Save neural network to file
        actor.save("actor_network_params.txt");
        critic.save("critic_network_params.txt");
    }
}

int main() {
    LinearRegression env;
    NeuralNetwork actor;
    NeuralNetwork critic;

    trainCartPolePolicyGradient(actor, critic, env, 1000, 0.99, 0.01, 0.1, 1e-4);

    return 0;
}