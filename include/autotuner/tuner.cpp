// im going to make my prototype of this using very basic gd. In the future, plan on using ADAMW to preform gd for the gains

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include "../pros/rtos.hpp"
#include "../pros/motors.hpp"
#include "../lemlib/pid.hpp"
#include "../lemlib/chassis/chassis.hpp"

struct GradientDescent
{
    double learningRate;
    std::vector<double> gradients;
    std::vector<double> last_losses;
    GradientDescent(double learningRate) : learningRate(learningRate) {}
    void update(std::vector<double> &params, const std::vector<double> &losses) {
        if (gradients.size()!= params.size()) {
            throw std::invalid_argument("Gradients and losses must have the same size");
        }
        while (!(gradients.size() == params.size())) {
            gradients.push_back(0.0f);
        }
        while (!(last_losses.size() == losses.size())) {
            last_losses.push_back(0.0f);
        }
        for (int k = 0; k < losses.size(); ++k) {
            gradients[k] = last_losses[k] - losses[k];
        }
        last_losses = losses;
        for (int i = 0; i < params.size(); ++i)
        {
            params[i] -= learningRate * gradients[i];
        }
    }
};

void write_file(std::vector<double> &gradients, std::vector<double> &params, std::string filename)
{
    // check the validity of the file name
    if (!filename.starts_with("/usd/")) {
        throw std::invalid_argument("File name must start with '/usd/'");
    }
    
    if (!filename.ends_with(".txt")) {
        throw std::invalid_argument("File name must end with '.txt'");
    }
    
    // make sure the vectors have the same size
    if (gradients.size() != params.size()) {
        throw std::invalid_argument("Gradients and parameters must have the same size");
    }
    
    // take the mutex
    pros::Mutex mtx;
    mtx.take();
    std::ofstream file(filename);
    
    // check to makesure the file is open
    if (!file) {
        mtx.give();
        throw std::runtime_error("Could not open file for writing");
    }
    
    // write the gradients and parameters to the file
    for (int i = 0; i < gradients.size(); ++i) {
        file << gradients[i] << " " << params[i] << "\n";
    }
    
    file.close();
    mtx.give();
}

std::pair<std::vector<double>, std::vector<double>> read_file(std::string filename) {
    // check the validity of the file name
    if (!filename.starts_with("/usd/")) {
        throw std::invalid_argument("File name must start with '/usd/'");
    }

    if (!filename.ends_with(".txt")) {
        throw std::invalid_argument("File name must end with '.txt'");
    }

    std::vector<double> gradients;
    std::vector<double> params;
    std::string line;
    // take the mutex
    pros::Mutex mtx;
    mtx.take();
    std::ifstream file(filename);
    // check to makesure the file is open
    if (!file) {
        mtx.give();
        throw std::runtime_error("Could not open file for reading");
    }

    // read the gradients and parameters from the file
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double gradient, param;
        if (!(iss >> gradient >> param)) {
            mtx.give();
            throw std::runtime_error("Invalid format in the file");
        }
        gradients.push_back(gradient);
        params.push_back(param);
    }
    file.close();
    mtx.give();
    return std::make_pair(gradients, params);
}

struct PID_Loss {
    double p_loss, i_loss, d_loss, total_loss;
};

PID_Loss compute_pid_losses(const std::vector<double>& actual_position, const std::vector<double>& desired_position, const std::vector<double>& control_effort, double dt, double kP, double kI, double kD, double wP, double wI, double wD) {
    double p_loss, i_loss, d_loss = 0.0f;
    double cumulative_error, previous_error = 0.0f;
    for (int i = 0; i < actual_position.size(); ++i) {
        double error = desired_position[i] - actual_position[i];
        cumulative_error += error * dt;
        // squared error for proportional loss
        p_loss = error * error;
        // squared cumulative error for integral loss
        i_loss = cumulative_error * cumulative_error;
        // squared change in error for derivative loss
        if (i > 0) {
            double derivative_loss = (error - previous_error) / dt;    
            d_loss = (error - previous_error) / dt;
        }
        previous_error = error;
    }
    double total_loss = wP * kP * p_loss + wI * kI * i_loss + wD * kD * d_loss;
    p_loss *= wP;
    i_loss *= wI;
    d_loss *= wD;
    return {p_loss, i_loss, d_loss, total_loss};
}

void tune_feedforward (lemlib::Chassis& chassis, std::vector<float> velocities, bool tune_angular = 0) {
    lemlib::PID feed_fowrward_pid(0.1, 0, 0, 0); // a placeholder until 0.6 comes out with FAPID
    lemlib::PID regular_pid = tune_angular ? chassis.angularPID : chassis.lateralPID; // make sure F gain is zero
    if (!tune_angular) {
        for (int i = 0; i < velocities.size(); ++i) {
            std::clamp(velocities[i], 1.0f, 126.0f); // ensure that velocities are in range
            chassis.moveToPoint(10000, 0, 100000, {.maxSpeed=velocities[i+1], .minSpeed=velocities[i-1]});
            pros::delay(500); // let the drive warm up to its velocity
            /*
            // find a method to get the input to the regular PID
            double output = regular_pid.get_output();
            //double ff_output = feed_fowrward_pid.update();
            //double difference = output - ff_output;
            //while (difference > 0.0) {
                feed_fowrward_pid.F_gain[i] += 0.01; // increment F gain by 0.01 until the difference is less than 0.0
            }
            */
        }
    } else {
        // figure this out later, projecly after the fork 
    }
}

// needs the main implementation of motor input along with LemLib's PID control. Maybe in a new file due to complexity?
// tune the dynamic weights
// check over the methods for comuting gradients