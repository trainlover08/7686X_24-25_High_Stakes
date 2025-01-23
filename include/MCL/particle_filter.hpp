#include <algorithm>
#include <cmath>
#include <vector>
#include <random>
#include <limits>
#include <iostream>
#include <stdexcept>

namespace mcl {
    struct Sensor {
        double x, y, theta; // Position and orientation
        double distance;    // Measured distance

        Sensor(double x, double y, double theta) : x(x), y(y), theta(theta), distance(0.0) {}
    };

    class Robot {
    public:
        std::vector<Sensor> sensors; // List of sensors
        double x, y, theta;          // Robot's position and orientation

        Robot(double x, double y, double theta, const std::vector<Sensor>& sensors)
            : x(x), y(y), theta(theta), sensors(sensors) {}
    };

    class Particle {
    public:
        double x, y, theta, fitness, weight;
        Robot* robot;

        Particle(double x, double y, double theta, Robot* robot)
            : x(x), y(y), theta(theta), fitness(0.0), weight(1.0), robot(robot) {}

        void update(double new_x, double new_y, double new_theta) {
            x = new_x;
            y = new_y;
            theta = normalize_angle(new_theta);
        }

        static double normalize_angle(double angle) {
            while (angle < 0) angle += 2 * M_PI;
            while (angle >= 2 * M_PI) angle -= 2 * M_PI;
            return angle;
        }
    };

    class ParticleFilter {
    public:
        ParticleFilter(Robot* robot, unsigned int num_particles)
            : robot(robot), num_particles(num_particles), gen(std::random_device{}()) {
            initialize_particles();
        }

        void predict(double velocity, double angular_velocity, double delta_t) {
            for (auto& particle : particles) {
                // Predict particle's new state
                particle.theta += angular_velocity * delta_t;
                particle.theta = Particle::normalize_angle(particle.theta);
                particle.x += velocity * std::cos(particle.theta) * delta_t;
                particle.y += velocity * std::sin(particle.theta) * delta_t;
            }
        }

        void update() {
            double total_weight = 0.0;

            // Update weights based on sensor readings
            for (auto& particle : particles) {
                particle.fitness = compute_fitness(particle);
                particle.weight = std::exp(-particle.fitness); // Convert fitness to weight
                total_weight += particle.weight;
            }

            // Normalize weights
            for (auto& particle : particles) {
                particle.weight /= total_weight;
            }

            resample_particles();
        }

    private:
        Robot* robot;
        unsigned int num_particles;
        std::vector<Particle> particles;
        std::mt19937 gen;

        void initialize_particles() {
            std::uniform_real_distribution<double> dist_x(0.0, 144.0);
            std::uniform_real_distribution<double> dist_y(0.0, 144.0);
            std::uniform_real_distribution<double> dist_theta(0.0, 2 * M_PI);

            for (unsigned int i = 0; i < num_particles; ++i) {
                particles.emplace_back(dist_x(gen), dist_y(gen), dist_theta(gen), robot);
            }
        }

        double compute_fitness(const Particle& particle) {
            double loss = 0.0;

            for (const auto& sensor : robot->sensors) {
                double predicted_distance = compute_distance(particle, sensor);
                loss += std::pow(sensor.distance - predicted_distance, 2);
            }

            return loss;
        }

        double compute_distance(const Particle& particle, const Sensor& sensor) {
            // Compute the predicted distance based on particle position and sensor orientation
            double sensor_x = particle.x + sensor.x * std::cos(particle.theta) - sensor.y * std::sin(particle.theta);
            double sensor_y = particle.y + sensor.x * std::sin(particle.theta) + sensor.y * std::cos(particle.theta);

            double sin_theta = std::sin(sensor.theta + particle.theta);
            double cos_theta = std::cos(sensor.theta + particle.theta);

            return calculate_wall_intersection(sensor_x, sensor_y, sin_theta, cos_theta);
        }

        double calculate_wall_intersection(double sensor_x, double sensor_y, double sin_theta, double cos_theta) {
            // Calculate intersection distance with walls (bounding box: [0, 144])
            const double field_min = 0.0;
            const double field_max = 144.0;
            std::vector<double> distances;

            // Check intersections with field boundaries
            if (cos_theta != 0) {
                double distance_x = (cos_theta > 0 ? field_max - sensor_x : field_min - sensor_x) / cos_theta;
                double y_intersection = sensor_y + distance_x * sin_theta;
                if (y_intersection >= field_min && y_intersection <= field_max) {
                    distances.push_back(std::abs(distance_x));
                }
            }

            if (sin_theta != 0) {
                double distance_y = (sin_theta > 0 ? field_max - sensor_y : field_min - sensor_y) / sin_theta;
                double x_intersection = sensor_x + distance_y * cos_theta;
                if (x_intersection >= field_min && x_intersection <= field_max) {
                    distances.push_back(std::abs(distance_y));
                }
            }

            return distances.empty() ? std::numeric_limits<double>::infinity() : *std::min_element(distances.begin(), distances.end());
        }

        void resample_particles() {
            std::vector<Particle> new_particles;
            std::vector<double> cumulative_weights;

            // Compute cumulative weights
            double cumulative_sum = 0.0;
            for (const auto& particle : particles) {
                cumulative_sum += particle.weight;
                cumulative_weights.push_back(cumulative_sum);
            }

            std::uniform_real_distribution<double> dist(0.0, 1.0);
            for (unsigned int i = 0; i < num_particles; ++i) {
                double random_value = dist(gen) * cumulative_sum;
                auto it = std::lower_bound(cumulative_weights.begin(), cumulative_weights.end(), random_value);
                new_particles.push_back(particles[it - cumulative_weights.begin()]);
            }

            particles = std::move(new_particles);
        }
    };
}