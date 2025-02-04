#include <cmath>
#include <vector>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <algorithm>

// ---------- Utility 2D Vector ---------- //
struct Vec2 {
    double x, y;

    Vec2() : x(0.0), y(0.0) {}
    Vec2(double _x, double _y) : x(_x), y(_y) {}

    Vec2 operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    Vec2 operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    Vec2 operator*(double s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(double s) const { return Vec2(x / s, y / s); }
};

// ---------- Hermite Spline for 2D ---------- //
class HermiteSpline2D {
public:
    struct SplineSegment {
    public:
        Vec2 p0, p1;  // Waypoints (start and end)
        Vec2 m0, m1;  // Tangents (first derivatives) at p0 and p1

        // Hermite basis functions
        constexpr double H00(double t) const { return 2*t*t*t - 3*t*t + 1; }
        constexpr double H10(double t) const { return t*t*t - 2*t*t + t; }
        constexpr double H01(double t) const { return -2*t*t*t + 3*t*t; }
        constexpr double H11(double t) const { return t*t*t - t*t; }

        // First derivative basis functions
        constexpr double dH00(double t) const { return 6*t*t - 6*t; }
        constexpr double dH10(double t) const { return 3*t*t - 4*t + 1; }
        constexpr double dH01(double t) const { return -6*t*t + 6*t; }
        constexpr double dH11(double t) const { return 3*t*t - 2*t; }

        // Second derivative basis functions
        constexpr double ddH00(double t) const { return 12*t - 6; }
        constexpr double ddH10(double t) const { return 6*t - 4; }
        constexpr double ddH01(double t) const { return -12*t + 6; }
        constexpr double ddH11(double t) const { return 6*t - 2; }

        // Evaluate the spline position at parameter t ∈ [0,1]
        Vec2 evaluate(double t) const {
            return p0 * H00(t) +
                   m0 * H10(t) +
                   p1 * H01(t) +
                   m1 * H11(t);
        }

        // Evaluate first derivative (velocity vector) at t
        Vec2 derivative(double t) const {
            return p0 * dH00(t) +
                   m0 * dH10(t) +
                   p1 * dH01(t) +
                   m1 * dH11(t);
        }

        // Evaluate second derivative (acceleration vector) at t
        Vec2 secondDerivative(double t) const {
            return p0 * ddH00(t) +
                   m0 * ddH10(t) +
                   p1 * ddH01(t) +
                   m1 * ddH11(t);
        }

        // Compute curvature at t.
        // For a 2D parametric curve, curvature κ = |x'y'' - y'x''| / ( (x'^2 + y'^2)^(3/2) )
        double curvature(double t) const {
            Vec2 d = derivative(t);
            Vec2 dd = secondDerivative(t);
            double numerator = std::abs(d.x * dd.y - d.y * dd.x);
            double denominator = std::pow(d.x*d.x + d.y*d.y, 1.5);
            return (denominator > 1e-6) ? numerator / denominator : 0.0;
        }
    };

    std::vector<SplineSegment> segments;

    // Construct a spline given 2D waypoints and corresponding tangents.
    // Here, waypoints and tangents are provided as vectors of Vec2.
    HermiteSpline2D(const std::vector<Vec2>& points, const std::vector<Vec2>& tangents) {
        if (points.size() != tangents.size() || points.size() < 2) {
            throw std::invalid_argument("Mismatched or insufficient points/tangents.");
        }
        for (size_t i = 0; i < points.size() - 1; i++) {
            segments.push_back({ points[i], points[i+1], tangents[i], tangents[i+1] });
        }
    }

    // Evaluate the spline at a normalized parameter x over all segments.
    // x in [0, numSegments], where integer part selects the segment and fractional part is the local t.
    Vec2 getPosition(double x) const {
        int idx = static_cast<int>(x);
        double t = x - idx;
        if (idx < 0) idx = 0;
        if (idx >= static_cast<int>(segments.size())) idx = segments.size() - 1;
        return segments[idx].evaluate(t);
    }

    // Compute curvature at a given normalized parameter x.
    double getCurvature(double x) const {
        int idx = static_cast<int>(x);
        double t = x - idx;
        if (idx < 0) idx = 0;
        if (idx >= static_cast<int>(segments.size())) idx = segments.size() - 1;
        return segments[idx].curvature(t);
    }
};

// ---------- Particle Structure ---------- //
struct particle {
    HermiteSpline2D spline;
    // Additional properties (like weight) can be added here.
};

// ---------- Differential Drive & Dynamics ---------- //
// Stub: Compute the required torque for a differential drive robot at a given speed and curvature.
// For example, one simple approximation might use centripetal force: F = m * v^2 / r,
// and then relate that force to a required torque on the wheels.
// Here, effectiveTurningRadius can be considered as a scaling factor (e.g., half the track width).
double computeRequiredTorque(double speed, double curvature) {
    constexpr double mass = 10.0;          // robot mass in kg (example)
    constexpr double effectiveTurningRadius = 0.3; // in meters (example)
    // Avoid division by zero: if curvature is near zero, torque is near zero.
    if (curvature < 1e-6) return 0.0;
    double radius = 1.0 / curvature; // turning radius
    // Compute centripetal force F = m * v^2 / r
    double force = mass * speed * speed / radius;
    // Convert force to torque: T = F * effectiveTurningRadius
    double torque = force * effectiveTurningRadius;
    return torque;
}

// ---------- Filter Class: Evaluate Particle Legality ---------- //
class Filter {
public:
    // Constructor uses 2D waypoints (each waypoint has x,y,theta for instance) to do spline fitting.
    // For now, we simply check the format.
    Filter(const std::vector<std::vector<double>>& waypoints) {
        if (waypoints.empty() || waypoints[0].size() != 3) {
            throw std::invalid_argument("Invalid waypoint format for filter. Expected 3 values per waypoint.");
        }
        // Actual spline fitting from waypoints would be implemented here.
    }

    // Stub for mutating a particle (to be implemented later)
    double mutate_particle(particle& p) {
        // @todo: Implement mutation logic
        return 0.0;
    }

    // This function calculates a fitness (cost) for a given particle by:
    //  - Sampling along the spline.
    //  - Computing curvature at each sample.
    //  - Estimating a safe velocity profile based on curvature.
    //  - Calculating the required torque for that velocity and comparing it against a maximum.
    //  - Penalizing deviations from the safe profile.
    //
    // Parameters:
    //   desiredSpeed: The robot’s nominal speed if no curvature limit were present.
    double calculate_fitness(const particle& p, double desiredSpeed) {
        // Robot and environment parameters
        constexpr double mu = 0.8;                // friction coefficient
        constexpr double g = 9.81;                // gravitational acceleration (m/s^2)
        constexpr double maxLateralAccel = mu * g;  // maximum allowable lateral (centripetal) acceleration
        constexpr double maxTorque = 50.0;          // maximum allowable torque (example, in Nm)

        double cost = 0.0;
        const size_t numSegments = p.spline.segments.size();
        if (numSegments == 0) return std::numeric_limits<double>::infinity();

        constexpr size_t samples = 200;
        const double x_max = static_cast<double>(numSegments);
        const double dx = x_max / samples;

        for (double x = 0.0; x <= x_max; x += dx) {
            double curv = p.spline.getCurvature(x);

            // Compute allowed maximum speed based on curvature.
            // If curvature is non-zero, v_max = sqrt(maxLateralAccel / curvature).
            // If curvature is near zero, allow up to desiredSpeed (or a preset maximum).
            double allowedSpeed = (curv > 1e-6) ? std::sqrt(maxLateralAccel / curv) : desiredSpeed;
            // For a safety margin, you might take a fraction of allowedSpeed.
            allowedSpeed = std::min(allowedSpeed, desiredSpeed);

            // Choose a local speed: if desiredSpeed is above allowedSpeed, you must slow down.
            double localSpeed = std::min(desiredSpeed, allowedSpeed);

            // Compute lateral (centripetal) acceleration: a_lat = v^2 * curvature.
            double lateralAccel = localSpeed * localSpeed * curv;
            if (lateralAccel > maxLateralAccel) {
                cost += (lateralAccel - maxLateralAccel) * 1000.0; // heavy penalty for unsafe lateral acceleration.
            }

            // Compute required torque for the local speed and curvature.
            double requiredTorque = computeRequiredTorque(localSpeed, curv);
            if (requiredTorque > maxTorque) {
                cost += (requiredTorque - maxTorque) * 1000.0; // heavy penalty for excessive torque.
            }

            // Add a smoothness penalty: high curvature segments add cost.
            cost += curv * curv;

            // Add a penalty if the local speed is significantly lower than the desired speed.
            // (i.e. if the curvature forces you to slow down, that might be suboptimal for timing.)
            cost += (desiredSpeed - localSpeed) * (desiredSpeed - localSpeed);
        }
        return cost;
    }
};

// ---------- Main: Demonstrate usage ---------- //
int main() {
    try {
        // Define some 2D waypoints for the spline.
        // For example: {x, y} coordinates.
        std::vector<Vec2> waypoints = {
            {0.0, 0.0},
            {2.0, 3.0},
            {5.0, 4.0},
            {7.0, 6.0}
        };

        // Define corresponding tangents (as Vec2).
        // In practice these might be estimated automatically, but we supply sample values here.
        std::vector<Vec2> tangents = {
            {1.0, 0.5},
            {1.5, 0.5},
            {0.5, 1.0},
            {0.0, 0.0}
        };

        // Create a HermiteSpline2D and wrap it in a particle.
        particle p { HermiteSpline2D(waypoints, tangents) };

        // Create a Filter with dummy waypoints (format: x,y,theta).
        std::vector<std::vector<double>> filterWaypoints = {
            {0.0, 0.0, 0.0},
            {2.0, 3.0, 45.0},
            {5.0, 4.0, 30.0},
            {7.0, 6.0, 0.0}
        };
        Filter filter(filterWaypoints);

        // Define a desired speed (in m/s).
        constexpr double desiredSpeed = 2.0;

        // Calculate and output the fitness (cost) for this particle.
        double fitness = filter.calculate_fitness(p, desiredSpeed);
        std::cout << "Calculated fitness (cost) for the path: " << fitness << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}