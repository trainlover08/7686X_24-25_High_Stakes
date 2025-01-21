#include <algorithm>
#include <cmath>
#include <vector>
#include <bitset>
#include <random>
#include "../lemlib/util.hpp"

//things to try on this project in the future
    //gradient decent for particle location

namespace mcl {
    struct sensor {
        double x, y, theta;
        double distance;

        sensor(double x, double y, double theta) : x(x), y(y), theta(theta) {}
    };

    class Robot {
    public:
        std::vector<sensor*> sensors;
        std::vector<std::tuple<double>> particles;
        double x, y, theta;

        Robot(std::vector<sensor*>) : sensors(std::vector<sensor*>()) {}
    };

    class Particle {
    public:
        Particle(double x, double y, double theta, Robot* robot) : x(x), y(y), theta(theta), robot(robot) {}

        double x, y, theta, fitness;
        double last_x, last_y = 0.0;
        double last_theta = 0.0;
        double delta_distance () {return std::hypot(x, y) - std::hypot(last_x, last_y);}
        double delta_angle () {return theta - last_theta;}
        Robot* robot;

        void update (double x_in, double y_in, double theta_in) {
            last_x = x;
            last_y = y;
            last_theta = theta;

            x = x_in;
            y = y_in;
            theta = theta_in;
        }
    };

    /* class ParticleFilter {
        ParticleFilter(Robot* robot, std::bitset<(14400*14400)>& bitmap, unsigned int points) : robot(robot), bitmap(bitmap), points(points) {
            Particle default_particle(7200, 7200, 0.0f, robot);
            std::vector<Particle> ps(points, default_particle);

            for (int i = 0; i < ps.size(); ++i) {
                mutate_particle(ps[i], 144.0f, 144.0f);
            }
        }

        Robot* robot;
        std::bitset<(14400*14400)>& bitmap;

        void update () {
            for (int j = 0; j < ps.size(); ++j) {
                std::vector<double> sensor_predictions = calculated_intercept(ps[j]);
                
                double loss = 0.0f;
                for (int i = 0; i < sensor_predictions.size(); ++i) {
                    loss += calculate_loss(ps[j].robot->sensors[i]->distance, sensor_predictions[i]);
                }

                ps[j].fitness = loss;
            }

            std::vector<Particle> mc_selection = monte_carlo(ps, points / 5);

            for (int k = 0; k < mc_selection.size(); ++k) {
                for (int m = 0; m < 5; ++m) {
                    ps[k * (points / 5) + m] = mc_selection[k];
                }
            }

            for (int n = 0; n < mc_selection.size(); ++n) {
                mutate_particle(ps[n], 144.0f, 144.0f);
            }
        }

    private:
        unsigned int points;
        std::vector<Particle> ps;

        double find_distance (double x, double y, double theta, double step_size) {
            // trig to find distance
            // ray-casting
            double direction_x = cos(theta);
            double direction_y = sin(theta);

            x *= 100;
            y *= 100;

            double current_x = x;
            double current_y = y;

            double t = 0.0f;
            while (1) {
                t += step_size;

                current_x = x + t * direction_x;
                current_y = y + t * direction_y;

                int grid_coords_x = static_cast<unsigned int>(current_x);
                int grid_coords_y = static_cast<unsigned int>(current_y);

                if (grid_coords_x < 0 || grid_coords_x >= 14400 || grid_coords_y < 0 || grid_coords_y >= 14400) {
                    break;
                }

                if (bitmap.test(14400 * grid_coords_y + grid_coords_x)) {
                    return t / 100;
                }
            }

            return -500;  // if I see a negitive number, then I know something aint right
        }

        std::vector<double> calculated_intercept (Particle& p) {
            std::vector<double> output;

            // Calculate the intercepts for each sensor on the robot's path from the particle
            for (unsigned int i = 0; i < p.robot->sensors.size(); i++) {
                // trig to find the x, y, heading set relative to the sensor location and tracking center
                double center_x = p.x;
                double center_y = p.y;
                double center_t = p.theta;

                double sensor_x = p.robot->sensors[i]->x;
                double sensor_y = p.robot->sensors[i]->y;
                double sensor_t = p.robot->sensors[i]->theta;

                // rotate the sensor coords so that they are reltiave to the robot tracking
                double rotated_x = sensor_x * cos(center_t) - sensor_y * sin(center_t);
                double rotated_y = sensor_x * sin(center_t) + sensor_y * cos(center_t);

                double field_x = rotated_x + center_x;
                double field_y = rotated_y + center_y;

                double field_theta = lemlib::sanitizeAngle(center_t + sensor_t, false);
                
                // find the distance to the nearest object
                double distance = find_distance(field_x, field_y, field_theta, 10.0f);

                output.push_back(distance);
            }
            return output;
        }

        double calculate_loss (double e_distance, double o_distance) {
            return pow(fabs(o_distance - e_distance), 2);
        }

        std::vector<Particle> monte_carlo (std::vector<Particle> pool, unsigned int breeders) {
            std::sort(pool.begin(), pool.end(), [](const Particle& a, const Particle& b) {return -a.fitness > -b.fitness;});
            std::vector<Particle> output;
            for (int j = 0; j < breeders; ++j) {
                output.push_back(pool[j]);
            }
            return output;
        }

        void mutate_particle (Particle& p, double map_width, double map_height) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> position_distribution(-p.delta_distance(), p.delta_distance());
            std::uniform_real_distribution<> angle_distribution(-p.delta_angle(), p.delta_angle());

            double x = p.x;
            double y = p.y;
            double theta = p.theta;

            x += position_distribution(gen);
            y += position_distribution(gen);
            theta += angle_distribution(gen);

            x = std::clamp(p.x, 0.0, map_width);
            y = std::clamp(p.y, 0.0, map_height);

            theta = lemlib::sanitizeAngle(p.theta, 0);

            p.update(x, y, theta);
        }
    };*/
    // need completely new filter class
};