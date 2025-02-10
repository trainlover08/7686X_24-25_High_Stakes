// test_filter.cpp
#include "filter.hpp"  // your header
#include <cassert>
#include <cmath>
#include <iostream>
#include <array>
#include <vector>

// --- Dummy implementations for testing ---

// Dummy for pros::Distance: minimal implementation.
namespace pros {
    class Distance {
    public:
        // For testing, simply return a constant raw distance.
        virtual double get() const { return 100.0; }
        virtual ~Distance() = default;
    };
}

// Dummy for lemlib::Pose: a minimal pose with public x, y, theta.
namespace lemlib {
    class Pose {
    public:
        double x, y, theta;
        Pose(double x_val = 0.0, double y_val = 0.0, double theta_val = 0.0)
            : x(x_val), y(y_val), theta(theta_val) {}
    };
}

// --- Tests for helper namespace functions ---

void testBubbleSortHelper() {
    // Suppose our helper::bubble_sort returns an array of indices that sort the input.
    constexpr std::array<int, 5> arr = { 4, 2, 5, 1, 3 };
    constexpr auto sortedIndices = helper::bubble_sort(arr);
    // Expected: the smallest element (1) is at index 3, then 2 (index 1), 3 (index 4), 4 (index 0), 5 (index 2).
    constexpr std::array<int, 5> expected = {3, 1, 4, 0, 2};
    for (std::size_t i = 0; i < expected.size(); i++) {
        assert(sortedIndices[i] == expected[i]);
    }
}

void testFindIntercept() {
    // Test helper::find_intercept for a known configuration.
    // For example, if you shoot a ray from (72,72) at 45 degrees in a 144x144 square,
    // the intersection might be (144,144) or very close.
    constexpr auto intercept = helper::find_intercept(45.0, 72.0, 72.0);
    const double tol = 1e-6;
    // Check that at least one coordinate is near 144.
    assert(std::fabs(intercept[0] - 144.0) < tol || std::fabs(intercept[1] - 144.0) < tol);
}

void testCalculateDistance() {
    // Using points (0,0) and (3,4) should give a distance of 5.
    constexpr std::array<std::array<double, 2>, 2> pts = {{{0.0, 0.0}, {3.0, 4.0}}};
    constexpr double dist = helper::calculate_distance(pts);
    const double tol = 1e-6;
    assert(std::fabs(dist - 5.0) < tol);
}

void testCalculateLoss() {
    // Without knowing the formula, check that a sample call returns a nonnegative value.
    double loss = helper::calculate_loss(10.0, 12.0, 5);
    assert(loss >= 0.0);
}

// --- Tests for Filter classes ---

void testParticleFunctions() {
    // Create a Particle and test its update and grad methods.
    Filter::Particle particle(0.0, 0.0, 0.0);
    // For testing, assume update sets the first set of pose variables.
    particle.update(1.0, 2.0, 0.5);
    const double tol = 1e-6;
    assert(std::fabs(particle.x_1 - 1.0) < tol);
    assert(std::fabs(particle.y_1 - 2.0) < tol);
    assert(std::fabs(particle.theta_1 - 0.5) < tol);

    // Call grad (expecting an array of 3 doubles). We cannot predict the values,
    // but we can at least check that it returns 3 elements.
    auto grad = particle.grad();
    static_assert(std::tuple_size<decltype(grad)>::value == 3, "grad should return 3 values");
}

void testSensorFunctions() {
    // Create a Sensor using our dummy Distance.
    pros::Distance* dummyDistance = new pros::Distance();
    Filter::Sensor sensor(dummyDistance, 1.0, 1.0, 0.0);
    double raw = sensor.get_raw();
    double dist = sensor.distance(raw);
    // Since dummyDistance returns 100.0, raw should be 100.0 (or processed similarly)
    assert(raw >= 0.0);
    assert(dist >= 0.0);
    delete dummyDistance;
}

void testRobotAndMonteCarlo() {
    // Create a dummy pose.
    lemlib::Pose* odom = new lemlib::Pose(0.0, 0.0, 0.0);
    // Create a sensor and add it to a vector.
    pros::Distance* dummyDistance = new pros::Distance();
    Filter::Sensor* sensor = new Filter::Sensor(dummyDistance, 0.0, 0.0, 0.0);
    std::vector<Filter::Sensor*> sensors = { sensor };
    
    // Create a Robot.
    Filter::Robot* robot = new Filter::Robot(odom, sensors);
    assert(robot->odom_pose == odom);
    assert(robot->sensor_vector.size() == 1);

    // Create a Monte_Carlo instance.
    Filter::Monte_Carlo* mc = new Filter::Monte_Carlo(robot);
    // For testing, fill the population with pointers to a dummy Particle.
    Filter::Particle* dummyParticle = new Filter::Particle(0.0, 0.0, 0.0);
    for (int i = 0; i < 100; ++i) {
        mc->population[i] = dummyParticle;
    }
    // Call select and breed functions.
    auto selected = mc->select(mc->population, robot);
    for (int i = 0; i < 20; ++i) {
        assert(selected[i] != nullptr);
    }
    auto bred = mc->breed(selected);
    for (int i = 0; i < 100; ++i) {
        assert(bred[i] != nullptr);
    }
    // Test weighted_average. (We cannot know the exact value, so we at least check type and basic consistency.)
    lemlib::Pose avgPose = mc->weighted_average(selected);
    (void)avgPose; // Suppress unused variable warning.

    // Clean up.
    delete dummyParticle;
    delete sensor;
    delete dummyDistance;
    delete robot;
    delete mc;
    delete odom;
}

void testMCLUpdate() {
    // Create a dummy pose.
    lemlib::Pose* pose = new lemlib::Pose(0.0, 0.0, 0.0);
    // Create dummy sensor info for MCL.
    std::vector<MCL::sensor> sensors;
    pros::Distance* dummyDistance = new pros::Distance();
    MCL::sensor sensorInstance { dummyDistance, 0.0, 0.0, 0.0 };
    sensors.push_back(sensorInstance);
    // Create an MCL instance.
    MCL mcl(pose, sensors);
    lemlib::Pose* updatedPose = mcl.update();
    assert(updatedPose != nullptr);
    // Clean up.
    delete dummyDistance;
    delete pose;
}

// --- Main test runner ---
int main() {
    std::cout << "Running tests...\n";

    testBubbleSortHelper();
    std::cout << "helper::bubble_sort passed.\n";

    testFindIntercept();
    std::cout << "helper::find_intercept passed.\n";

    testCalculateDistance();
    std::cout << "helper::calculate_distance passed.\n";

    testCalculateLoss();
    std::cout << "helper::calculate_loss passed.\n";

    testParticleFunctions();
    std::cout << "Filter::Particle functions passed.\n";

    testSensorFunctions();
    std::cout << "Filter::Sensor functions passed.\n";

    testRobotAndMonteCarlo();
    std::cout << "Filter::Robot and Filter::Monte_Carlo functions passed.\n";

    testMCLUpdate();
    std::cout << "MCL::update passed.\n";

    std::cout << "All tests passed successfully.\n";
    return 0;
}