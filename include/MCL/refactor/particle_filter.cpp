#include "particle_filter.hpp"
#include <random>
#include "constexpr_math.hpp"

MCL::MCL (lemlib::Pose* pose, std::vector<sensor> sensors) {
    sensor_vec.resize(sensors.size());
    for (int i = 0; i < sensors.size(); ++i) {
        sensor_vec[i] = new Filter::Sensor (sensors[i].d, sensors[i].x, sensors[i].y, sensors[i].theta);
    }
    robot = new Filter::Robot (pose, sensor_vec);
    mc = new Filter::Monte_Carlo (robot);
}

MCL::~MCL() {
    delete mc;
    delete robot;
    for (auto s : sensor_vec) {
        delete s;
    }
}

lemlib::Pose* MCL::update () {
    return mc->cycle();
}

constexpr Filter::Particle::Particle (double x, double y, double theta) {
    this->x_1 = x;
    this->y_1 = y;
    this->theta_1 = theta;
    this->x_2 = x;
    this->y_2 = y;
    this->theta_2 = theta;
}

constexpr void Filter::Particle::update (double x, double y, double theta) {
    this->x_2 = this->x_1;
    this->y_2 = this->y_1;
    this->theta_2 = this->theta_1;
    this->x_1 = x;
    this->y_1 = y;
    this->theta_1 = theta;
}

constexpr std::array<double, 3> Filter::Particle::grad () {
    return {this->x_1 - this->x_2, this->y_1 - this->y_2, this->theta_1 - this->theta_2};
}

Filter::Sensor::Sensor (pros::Distance* d, double x, double y, double theta) {
    this->d = d;
    this->x = x;
    this->y = y;
    this->theta = theta;
}

constexpr double Filter::Sensor::get_raw () {
    return d->get();
}

constexpr double Filter::Sensor::distance (double raw) {
    // add extra logic here

    // mm to inches
    return raw * 0.03937007874;
}

constexpr Filter::Robot::Robot (lemlib::Pose* odom_pose, std::vector<Sensor*> sensor_vector) {
    this->odom_pose = odom_pose;
    this->sensor_vector = sensor_vector;
}

constexpr std::array<double, 2> helper::find_intercept (double global_theta, double x, double y) {
    double theta_rad = cxprmath::deg_to_rad(global_theta);
    
    double cos_theta = cxprmath::cos(theta_rad);
    double sin_theta = cxprmath::sin(theta_rad);
    
    double t_x, t_y;
    if (cos_theta > 0) {
        t_x = (144.0 - x) / cos_theta;
    } else if (cos_theta < 0) {
        t_x = (0.0 - x) / cos_theta;
    } else {
        t_x = 1e9;
    }
    
    if (sin_theta > 0) {
        t_y = (144.0 - y) / sin_theta;
    } else if (sin_theta < 0) {
        t_y = (0.0 - y) / sin_theta;
    } else {
        t_y = 1e9;
    }
    
    // find first intercept
    double t = (t_x < t_y) ? t_x : t_y;

    /** @todo add ladder posts */
    
    return { x + t * cos_theta, y + t * sin_theta };
}

constexpr double helper::calculate_distance(std::array<std::array<double, 2>, 2> arr) {
    return cxprmath::hypot(arr[0][0] - arr[1][0], arr[0][1] - arr[1][1]);
}

constexpr double calculate_loss (double o, double e, int raw) {
    // squared error
    if (raw == 9999) {
        if (e < 2.0 * 0.00254) {
            return 1000.0;
        } 
    }
    return 1000.0 - cxprmath::pow(o - e, 2.0);
}

template <typename T, std::size_t N>
constexpr std::array<int, N> helper::bubble_sort(std::array<T, N> arr) {
    std::array<int, N> indices{};
    for (std::size_t i = 0; i < N; ++i) {
        indices[i] = static_cast<int>(i);
    }
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N - i - 1; ++j) {
            if (arr[indices[j]] > arr[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
    return indices;
}

constexpr std::array<Filter::Particle*, 20> Filter::Monte_Carlo::select (std::array<Filter::Particle*, 100> p_arr, Filter::Robot* r) {
    std::array<double, 100> fitness_arr;
    for (int i = 0; i < p_arr.size(); ++i) {
        p_arr[i]->fitness = 0.0;
        for(int j = 0; j < r->sensor_vector.size(); ++j) {
            Sensor* s = r->sensor_vector[j];
            // calculate the sensor location
            double field_x = p_arr[i]->x_1;
            double field_y = p_arr[i]->y_1;
            double field_theta = p_arr[i]->theta_1;
            
            double sensor_x = s->x;
            double sensor_y = s->y;
            double sensor_theta = s->theta;

            double rotated_x = sensor_x * cxprmath::cos(field_theta) - sensor_y * cxprmath::sin(field_theta);
            double rotated_y = sensor_x * cxprmath::sin(field_theta) + sensor_y * cxprmath::cos(field_theta);

            field_x += rotated_x;
            field_y += rotated_y;

            field_theta = cxprmath::clean_angle_degrees(field_theta + sensor_theta, true);

            std::array<double, 2> intercept = helper::find_intercept(field_theta, field_x, field_y);
            std::array<double, 2> robot_pose = {field_x, field_y};
            double distance = helper::calculate_distance({robot_pose, intercept});

            p_arr[i]->fitness += calculate_loss(s->distance(s->get_raw()), distance, s->get_raw());
        }
        p_arr[i]->fitness /= r->sensor_vector.size();
        fitness_arr[i] = p_arr[i]->fitness;
    }
    std::array<int, 100> sorted_indices = helper::bubble_sort(fitness_arr);
    std::array<Particle*, 20> selected_particles;
    for (int k = 0; k < 20; ++k) {
        selected_particles[k] = p_arr[sorted_indices[k]];
    }
    return selected_particles;
}

constexpr std::array<Filter::Particle*, 100> Filter::Monte_Carlo::breed (std::array<Filter::Particle*, 20> p_arr) {
    // create new
    std::array<Particle*, 100> new_particles;
    for (int i = 0; i < 100; ++i) {
        new_particles[i] = p_arr[i % 20];
    }
    return new_particles;
}

std::array<Filter::Particle*, 100> Filter::Monte_Carlo::mutate (std::array<Filter::Particle*, 100> p_arr) {
    double SCALE_FACTOR = 5.0;
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < 100; ++i) {
        Particle* p = p_arr[i];
        double grad_x = p->grad()[0];
        double grad_y = p->grad()[1];
        double grad_theta = p->grad()[2];
        std::normal_distribution<> x_distribution(-SCALE_FACTOR * grad_x, SCALE_FACTOR * grad_x);
        std::normal_distribution<> y_distribution(-SCALE_FACTOR * grad_y, SCALE_FACTOR * grad_y);
        std::normal_distribution<> angle_distribution(-SCALE_FACTOR * grad_theta, SCALE_FACTOR * grad_theta);

        p->update(p->x_1 + x_distribution(gen), p->y_1 + y_distribution(gen), p->theta_1 + angle_distribution(gen));
        p_arr[i] = p; // not sure if this line is needed?
    }
    return p_arr;
}

constexpr void Filter::Particle::update_for_odom(lemlib::Pose* new_pose) {
    if (last_pose) {
        double dx = new_pose->x - last_pose->x;
        double dy = new_pose->y - last_pose->y;
        double dtheta = cxprmath::clean_angle_degrees(new_pose->theta - last_pose->theta, true);
        this->x_1 += dx;
        this->y_1 += dy;
        this->theta_1 = cxprmath::clean_angle_degrees(this->theta_1 + dtheta, true);
        this->x_2 += dx;
        this->y_2 += dy;
        this->theta_2 = cxprmath::clean_angle_degrees(this->theta_2 + dtheta, true);
    }
    last_pose = new_pose;
}

constexpr lemlib::Pose* Filter::Monte_Carlo::weighted_average(std::array<Particle*, 20> p_arr) {
    double total_weight = 0.0;
    double x_sum = 0.0;
    double y_sum = 0.0;
    double sin_sum = 0.0;
    double cos_sum = 0.0;
    for (int i = 0; i < 20; ++i) {
        double weight = (p_arr[i]->fitness > 0) ? (1.0 / p_arr[i]->fitness) : 1000.0;
        total_weight += weight;
        x_sum += weight * p_arr[i]->x_1;
        y_sum += weight * p_arr[i]->y_1;
        sin_sum += weight * cxprmath::sin(p_arr[i]->theta_1);
        cos_sum += weight * cxprmath::cos(p_arr[i]->theta_1);
    }
    pose->x = x_sum / total_weight;
    pose->y = y_sum / total_weight;
    pose->theta = cxprmath::rad_to_degrees(cxprmath::atan2(sin_sum, cos_sum));
    return pose;
}

lemlib::Pose* Filter::Monte_Carlo::cycle () {
    for (int i = 0; i < 100; ++i) {
        population[i]->update_for_odom(pose);
    }
    std::array<Particle*, 20> best = Filter::Monte_Carlo::select(population, Monte_Carlo::robot);
    lemlib::Pose* p = Filter::Monte_Carlo::weighted_average(best);
    population = Filter::Monte_Carlo::breed(best);
    population = Filter::Monte_Carlo::mutate(population);
    return p;
}

Filter::Monte_Carlo::Monte_Carlo(Robot* robot) {
    this->robot = robot;
    this->pose = robot->odom_pose;
    // initialize particles
    for (int i = 0; i < 100; ++i) {
        Filter::Particle* p = new Filter::Particle(pose->x, pose->y, pose->theta);
        population[i] = p;
    }
    Monte_Carlo::mutate(population);
}

Filter::Monte_Carlo::~Monte_Carlo() {
    for (int i = 0; i < 100; ++i) {
        delete population[i];
    }
}

