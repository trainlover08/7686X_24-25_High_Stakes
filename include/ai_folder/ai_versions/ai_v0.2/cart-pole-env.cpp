#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

class LinearRegression {
    public:
    void reset(){
        done = false;
        pose = 0.0f;
        target = 10.0f;
        velocity = 0.0f;
        current_step = 0;
    }
    
    void step(double delta_v) {
        ++current_step;
        delta_v < 1.0 ? 1 : delta_v;
        delta_v > -1.0 ? -1 : delta_v;
        velocity += delta_v;
        pose += velocity;

        if (current_step = 25) {
            done = true;
        }
    }

    bool isDone() const {
        return done;
    }

    std::vector<double> getState() const {
        return {velocity, pose};
    }

    double getReward() {
        double reward = 0;
        reward -= std::pow(std::fabs(target - pose), 2);
        reward -= current_step;
        return reward;
    }

    private:
    bool done = false;
    double pose = 0.0f;
    double target = 10.0f;
    double velocity = 0.0f;
    int current_step = 0;

};

class CartPoleEnv {
public:
    CartPoleEnv()
        : gravity(9.8), massCart(1.0), massPole(0.1), length(0.5), 
          forceMag(10.0), tau(0.02), thetaThresholdRadians(12 * 2 * M_PI / 360),
          xThreshold(2.4), totalMass(massCart + massPole), polemassLength(massPole * length),
          state{0.0, 0.0, 0.0, 0.0}, done(false) {}

    void reset() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.05, 0.05);

        state[0] = dis(gen); // cart position
        state[1] = dis(gen); // cart velocity
        state[2] = dis(gen); // pole angle
        state[3] = dis(gen); // pole angular velocity

        done = false;
    }

    void step(int action) {
        double x = state[0];
        double x_dot = state[1];
        double theta = state[2];
        double theta_dot = state[3];

        double force = action;
        double costheta = cos(theta);
        double sintheta = sin(theta);

        double temp = (force + polemassLength * theta_dot * theta_dot * sintheta) / totalMass;
        double theta_acc = (gravity * sintheta - costheta * temp) / 
                           (length * (4.0 / 3.0 - massPole * costheta * costheta / totalMass));
        double x_acc = temp - polemassLength * theta_acc * costheta / totalMass;

        // Update state
        state[0] += tau * x_dot;
        state[1] += tau * x_acc;
        state[2] += tau * theta_dot;
        state[3] += tau * theta_acc;

        // Check termination
        done = (x < -xThreshold || x > xThreshold || theta < -thetaThresholdRadians || theta > thetaThresholdRadians);
    }

    bool isDone() const {
        return done;
    }

    std::vector<double> getState() const {
        return {state[0], state[1], state[2], state[3]};
    }

    double getReward() const {
        // Normalize cart position and pole angle
        double cartPositionNorm = 1.0 - (std::abs(state[0]) / xThreshold);
        double poleAngleNorm = 1.0 - (std::abs(state[2]) / thetaThresholdRadians);

        // Penalize for excessive cart velocity or pole angular velocity
        double cartVelocityPenalty = 1.0 - std::min(1.0, std::abs(state[1]) / 5.0); // Adjust the 5.0 based on acceptable limits
        double poleVelocityPenalty = 1.0 - std::min(1.0, std::abs(state[3]) / 5.0); // Adjust the 5.0 based on acceptable limits

        // Scale penalties to provide more emphasis on critical components
        double positionWeight = 1.0;
        double angleWeight = 2.0; // More emphasis on keeping the pole upright

        // Combine rewards and penalties
        double reward = (positionWeight * cartPositionNorm) * 
                        (angleWeight * poleAngleNorm) * 
                        cartVelocityPenalty * poleVelocityPenalty;

        // Ensure reward is not negative
        return std::max(0.0, reward);
    }

private:
    const double gravity;
    const double massCart;
    const double massPole;
    const double length;  // actually half the pole's length
    const double forceMag;
    const double tau;  // seconds between state updates
    const double thetaThresholdRadians;
    const double xThreshold;
    const double totalMass;
    const double polemassLength;

    double state[4];
    bool done;
};

class VelocityControlEnv {
public:
    VelocityControlEnv()
        : targetVelocity(1.0), maxPosition(5.0), maxVelocity(2.0), tau(0.02), done(false) {
        reset();
    }

    void reset() {
        position = 0.0;
        velocity = 0.0;
        done = false;
    }

    void step(int action) {
        double force = (action == 0) ? -1.0 : 1.0; // Action: 0 = decrease velocity, 1 = increase velocity
        double acceleration = force;  // Simplified acceleration model

        // Update velocity and position
        velocity += tau * acceleration;
        position += tau * velocity;

        // Apply velocity limits
        if (velocity > maxVelocity) velocity = maxVelocity;
        if (velocity < -maxVelocity) velocity = -maxVelocity;

        // Apply position limits
        if (position > maxPosition || position < -maxPosition) {
            done = true;
        }
    }

    bool isDone() const {
        return done;
    }

    std::vector<double> getState() const {
        return {position, velocity};
    }

    double getReward() const {
        // Reward is based on how close the velocity is to the target
        double velocityError = std::abs(velocity - targetVelocity);
        return std::max(0.0, 1.0 - velocityError / maxVelocity);  // Reward is normalized between 0 and 1
    }

private:
    double position;
    double velocity;
    const double targetVelocity;
    const double maxPosition;
    const double maxVelocity;
    const double tau;  // Time step
    bool done;
};
