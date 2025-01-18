#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <tuple>
#include <cassert>
#include <deque>
#include <limits>
#include "neural_network.cpp"
#include "cart-pole-env.cpp"

using namespace std;

// Hyperparameters for SAC
const double GAMMA = 0.95;         // Discount factor for rewards
const double TAU = 0.005;          // Soft update factor
const double LR = 0.0010;          // Learning rate for both actor and critic
const double ALPHA = 0.1;          // Entropy regularization factor
const size_t BATCH_SIZE = 4;      // Batch size for training
const size_t CAPACITY = 1e6;    // Capacity of the replay buffer
const double TARGET_UPDATE_INTERVAL = 1.0;  // Frequency of target network updates
const int TRAIN_INTERVAL = 10; // Number of steps before training

class SoftActorCritic {
private:
    NeuralNetwork actor;            // The actor (policy) network
    NeuralNetwork critic1;          // First critic network
    NeuralNetwork critic2;          // Second critic network
    NeuralNetwork targetCritic1;    // Target network for critic1
    NeuralNetwork targetCritic2;    // Target network for critic2
    AdamWOptimizer actorOptimizer;  // Optimizer for the actor
    AdamWOptimizer criticOptimizer1; // Optimizer for the first critic
    AdamWOptimizer criticOptimizer2; // Optimizer for the second critic
    ReplayBuffer replayBuffer;      // Replay buffer
    LinearRegression env;                // The environment

public:
    SoftActorCritic()
        : replayBuffer(CAPACITY),
          actorOptimizer(LR, 0.9, 0.999, 1e-8, 0.01),
          criticOptimizer1(LR, 0.9, 0.999, 1e-8, 0.01),
          criticOptimizer2(LR, 0.9, 0.999, 1e-8, 0.01),
          actor(), critic1(), critic2(), targetCritic1(), targetCritic2(), env() {

        // Define the actor and critic networks
        actor.add_layer(Layer(2, 64, "relu", actorOptimizer));  // Input: 4 (state), Output: 64 (hidden)
        actor.add_layer(Layer(64, 64, "relu", actorOptimizer));  // Hidden to hidden
        actor.add_layer(Layer(64, 64, "relu", actorOptimizer));  
        actor.add_layer(Layer(64, 1, "none", actorOptimizer));  // Output: action distribution (mean, log stddev)

        critic1.add_layer(Layer(2, 32, "relu", criticOptimizer1));  // State input to hidden
        critic1.add_layer(Layer(32, 32, "relu", criticOptimizer1)); // Hidden to hidden
        critic1.add_layer(Layer(32, 1, "none", criticOptimizer1));  // Output: Q-value

        critic2.add_layer(Layer(2, 32, "relu", criticOptimizer2));
        critic2.add_layer(Layer(32, 32, "relu", criticOptimizer2));
        critic2.add_layer(Layer(32, 1, "none", criticOptimizer2));

        targetCritic1 = critic1;
        targetCritic2 = critic2;
    }

    vector<double> sampleAction(const vector<double>& state) {
        // Forward pass through the actor network to get action distribution
        vector<vector<double>> inputState = {state};
        vector<vector<double>> actionParams = actor.forward(inputState);
        
        // Sample action from normal distribution
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<double> dist(actionParams[0][0], exp(actionParams[0][1]));

        double action = dist(gen);

        // Discretize action for CartPole (-1 or 1)
        // action = action >= 0 ? 1 : -1;

        return {action};  // Return the sampled action
    }

    void train() {
    if (!replayBuffer.isReady(BATCH_SIZE)) return;

    // Sample a batch of transitions from the replay buffer
    auto [states, actions, rewards, nextStates, dones] = replayBuffer.sampleBatch(BATCH_SIZE);

    vector<vector<double>> critic1LossGrads(BATCH_SIZE, vector<double>(1));
    vector<vector<double>> critic2LossGrads(BATCH_SIZE, vector<double>(1));
    vector<vector<double>> actorLossGrads(BATCH_SIZE, vector<double>(2));

    // Log batch rewards
    //cout << "Training Batch - Rewards: ";
    for (size_t i = 0; i < BATCH_SIZE; ++i) {
        //cout << rewards[i] << " ";
    }
    //cout << endl;

    // Calculate target Q-value: r + γ * (1 - done) * min(Q_target1, Q_target2)
    vector<vector<double>> targetQs(BATCH_SIZE, vector<double>(1));
    for (size_t i = 0; i < BATCH_SIZE; ++i) {
        vector<vector<double>> nextState = {nextStates[i]};
        double targetQ1 = targetCritic1.forward(nextState)[0][0];
        double targetQ2 = targetCritic2.forward(nextState)[0][0];
        double minTargetQ = min(targetQ1, targetQ2);
        targetQs[i][0] = rewards[i] + GAMMA * (1.0 - dones[i]) * minTargetQ;

        // Log target Q-values
        //cout << "Target Q[" << i << "]: " << targetQs[i][0] << endl;
    }

    // Forward pass through critic networks and calculate losses
    for (size_t i = 0; i < BATCH_SIZE; ++i) {
        vector<vector<double>> currentState = {states[i]};
        vector<vector<double>> action = {{(double)actions[i]}};

        double critic1Q = critic1.forward(currentState)[0][0];
        double critic2Q = critic2.forward(currentState)[0][0];

        critic1LossGrads[i][0] = critic1Q - targetQs[i][0];
        critic2LossGrads[i][0] = critic2Q - targetQs[i][0];

        // Log critic outputs
        //cout << "Critic1 Q[" << i << "]: " << critic1Q << " (Loss: " << critic1LossGrads[i][0] << ")" << endl;
        //cout << "Critic2 Q[" << i << "]: " << critic2Q << " (Loss: " << critic2LossGrads[i][0] << ")" << endl;
    }

    // Backward pass and weight updates for critics
    double grad_clip = 1.0;
    critic1.backward(critic1LossGrads, grad_clip);
    critic2.backward(critic2LossGrads, grad_clip);
    critic1.update_weights();
    critic2.update_weights();

    std::random_device rd;
    std::mt19937 gen(rd());

    // Calculate actor loss and update
    for (size_t i = 0; i < BATCH_SIZE; ++i) {
        vector<vector<double>> state = {states[i]};
        vector<vector<double>> actionParams = actor.forward(state);

        normal_distribution<double> dist(actionParams[0][0], exp(actionParams[0][1]));
        double sampled_action = dist(gen); // Sampled action
        double log_prob = safeLog(dist(gen));

        actorLossGrads[i][0] = ALPHA * log_prob - critic1.forward(state)[0][0];

        // Log actor outputs and loss
        //cout << "Actor - Sampled Action[" << i << "]: " << sampled_action << " Log Prob: " << log_prob << " Actor Loss: " << actorLossGrads[i][0] << endl;
    }

        // Backward pass and weight updates for the actor
        actor.backward(actorLossGrads, grad_clip);
        actor.update_weights();
    }

    void run() {
        int steps = 0;

        for (int episode = 0; episode <= 1500; ++episode) {
            env.reset();
            double totalReward = 0;
            while (!env.isDone()) {
                vector<double> state = env.getState();
                vector<double> action = sampleAction(state);

                env.step(action[0]);
                double reward = env.getReward();
                vector<double> nextState = env.getState();
                bool done = env.isDone();

                replayBuffer.storeTransition(state, (int)action[0], reward, nextState, done);
                totalReward += reward;

                steps++;
                if (steps % TRAIN_INTERVAL == 0) {
                    train();  // Train after every TRAIN_INTERVAL steps
                }
            }

            if(episode % 10 == 0) {
                cout << "Episode " << episode << " Total Reward: " << totalReward << endl;
            }
        }
    }
};

int main() {
    SoftActorCritic sac;
    sac.run();
    return 0;
}