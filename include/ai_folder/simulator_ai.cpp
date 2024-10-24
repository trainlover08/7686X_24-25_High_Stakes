#include "ai.cpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <utility>

class global_controller {
    public:
        bool is_breaking_rules;
        unsigned int episode_number;
        unsigned int epoch_number;
        bool game_reset;
        bool out_of_time;
};

class local_controller {
    public:
        double motor_voltage_left;
        double motor_voltage_right;
        double motor_velocity_left;
        double motor_velocity_right;
        double current_time;
        std::pair<double, double> odom_position; // {x, y}
        double odom_heading;
        bool mogo_state;
        bool claw_state;
        double lift_voltage;
        double lift_position;
        double intake_voltage;
        double intake_velocity;

        std::vector<double> dynamic_memory{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        
        std::vector<std::pair<double, unsigned char>> ring_distances_type; //to emulate the size function of the vision sensor
        
};

std::vector<std::pair<double, unsigned char>> distance_sort(const local_controller& controller) {
    std::vector<std::pair<double, unsigned char>> sorted_pairs = controller.ring_distances_type;

     // Sort the vector based on the double values in the pairs
    std::sort(sorted_pairs.begin(), sorted_pairs.end(), [](const std::pair<double, bool>& a, const std::pair<double, bool>& b) {
        return a.first < b.first; // Sort in ascending order based on the double values
    });

    // Resize the vector to keep only the top 10 pairs, if there are more than 10 elements
    if (sorted_pairs.size() > 11) {
        sorted_pairs.resize(11);
    }

    return sorted_pairs;
}

std::vector<double> prepare_inputs(const local_controller& local, const global_controller& global) {
    std::vector<double> input_vector;

    input_vector.push_back(local.motor_velocity_left);
    input_vector.push_back(local.motor_velocity_right);
    input_vector.push_back(local.current_time);
    input_vector.push_back(local.odom_position.first);
    input_vector.push_back(local.odom_position.second);
    input_vector.push_back(local.odom_heading);
    input_vector.push_back(local.motor_voltage_left);
    input_vector.push_back(local.motor_voltage_right);

    std::vector<std::pair<double, unsigned char>> sorted_distances = distance_sort(local);

    for(int dist_idx = 0; dist_idx < sorted_distances.size(); dist_idx++) {
        input_vector.push_back(sorted_distances[dist_idx].second);
    }

    for(int dist_val_idx = 0; dist_val_idx < sorted_distances.size(); dist_val_idx++) {
        input_vector.push_back(sorted_distances[dist_val_idx].first);
    }

    input_vector.push_back(local.mogo_state);
    input_vector.push_back(local.claw_state);
    input_vector.push_back(local.lift_voltage);
    input_vector.push_back(local.lift_position);
    input_vector.push_back(local.intake_velocity);
    input_vector.push_back(local.intake_voltage);

    for(int dyn_idx = 0; dyn_idx < local.dynamic_memory.size(); dyn_idx++) {
        input_vector.push_back(local.dynamic_memory[dyn_idx]);
    }
}

void ai_train(){
    ai::NeuralNetwork movement_nn;
    movement_nn.current_epoch = 0;
    movement_nn.FILENAME = "movement_nn_wb";
    movement_nn.selected_models = 4;
    movement_nn.episodes = 32;
    movement_nn.LEARNINGRATE = 2.5;
    unsigned int E_EPOCHS = 15000;

    global_controller global_controller;
    local_controller local_controller;

    movement_nn.createnewmodel(57, 80, 12, 27);

    /*
    Dimentions for the nn

    inputs:
        1 - velocity for left drive
        2 - velocity for right drive
        3 - current time
        4 - odom_position.x
        5 - odom_position.y
        6 - odom_heading
        7 - voltage for left drive
        8 - voltage for right drive

        *sorted in distance from bot
        9-19 - object type (bin)
        20-30 - object distance

        31 - MoGo Mech state
        32 - claw state
        33 - 4-bar lift voltage
        34 - 4-bar lift sensor position
        35 - intake motor speed
        36 - intake motor voltage
        
        37-57 - dynamic memory

    outputs:
        1 - left drive motor voltage
        2 - right drive motor voltage
        3 - lift motor voltage
        4 - intake motor voltage
        5 - MoGo mech state
        6 - claw state

        7-27 - dynamic memory
    
    */

    for(int current_epoch = 0; current_epoch < E_EPOCHS; ++current_epoch, ++global_controller.epoch_number) {
        auto mutated_batch = movement_nn.mutate();

        std::vector<std::vector<std::vector<double>>> nn_input;

        nn_input.resize(1);

        nn_input[0].resize(1);

        std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> agent_batch;

        agent_batch.resize(1);

        for(int agent_idx = 0; agent_idx < mutated_batch.size(); ++agent_idx) {
            agent_batch[0] = mutated_batch[agent_idx];
            for(int timestamp_idx = 0; timestamp_idx < 600; ++timestamp_idx) {
                nn_input[0][0] = prepare_inputs(local_controller, global_controller);
                movement_nn.run(agent_batch, nn_input);
            }
        }

        // movement_nn.run(mutated_batch, nn_input);

        // calculate fitness and update the model's fitness

        
    }

}