#include <cmath>
#include <vector>
#include <stdexcept>

constexpr double convert_angle (double x, bool input_type_radians) {
    if (input_type_radians) {
        return x / (M_PI / 180.0);
    } else {
        return x * M_PI / 180.0;
    }
}
class HermiteSpline {
public:
    struct SplineSegment {
    public:
        double p0, p1;  // Waypoints
        double m0, m1;  // Tangents (first derivatives)

        // Hermite basis functions
        constexpr double H00(double t) const { return 2 * t * t * t - 3 * t * t + 1; }
        constexpr double H10(double t) const { return t * t * t - 2 * t * t + t; }
        constexpr double H01(double t) const { return -2 * t * t * t + 3 * t * t; }
        constexpr double H11(double t) const { return t * t * t - t * t; }

        // First derivative basis functions
        constexpr double dH00(double t) const { return 6 * t * t - 6 * t; }
        constexpr double dH10(double t) const { return 3 * t * t - 4 * t + 1; }
        constexpr double dH01(double t) const { return -6 * t * t + 6 * t; }
        constexpr double dH11(double t) const { return 3 * t * t - 2 * t; }

        // Second derivative basis functions
        constexpr double ddH00(double t) const { return 12 * t - 6; }
        constexpr double ddH10(double t) const { return 6 * t - 4; }
        constexpr double ddH01(double t) const { return -12 * t + 6; }
        constexpr double ddH11(double t) const { return 6 * t - 2; }

        // Evaluate spline position at t
        constexpr double evaluate(double t) const {
            return H00(t) * p0 + H10(t) * m0 + H01(t) * p1 + H11(t) * m1;
        }

        // Compute first derivative (velocity) at t
        constexpr double derivative(double t) const {
            return dH00(t) * p0 + dH10(t) * m0 + dH01(t) * p1 + dH11(t) * m1;
        }

        // Compute second derivative (acceleration) at t
        constexpr double secondDerivative(double t) const {
            return ddH00(t) * p0 + ddH10(t) * m0 + ddH01(t) * p1 + ddH11(t) * m1;
        }

        // Compute curvature at t
        constexpr double curvature(double t) const {
            double f_prime = derivative(t);
            double f_double_prime = secondDerivative(t);
            return std::abs(f_double_prime) / std::pow(1 + f_prime * f_prime, 1.5);
        }
    };

    std::vector<SplineSegment> segments;

    // Construct spline given waypoints and optional tangents
    constexpr HermiteSpline(const std::vector<double>& points, const std::vector<double>& tangents) {
        if (points.size() != tangents.size() || points.size() < 2) {
            throw std::invalid_argument("Mismatched or insufficient points/tangents.");
        }
        for (size_t i = 0; i < points.size() - 1; i++) {
            segments.push_back({points[i], points[i + 1], tangents[i], tangents[i + 1]});
        }
    }

    // Evaluate the spline position at x (normalized index)
    constexpr double getPosition(double x) const {
        int idx = static_cast<int>(x);
        double t = x - idx;  // Fractional part for interpolation
        if (idx < 0) idx = 0;
        if (idx >= segments.size()) idx = segments.size() - 1;
        return segments[idx].evaluate(t);
    }

    // Compute curvature at a given x position
    constexpr double getCurvature(double x) const {
        int idx = static_cast<int>(x);
        double t = x - idx;
        if (idx < 0) idx = 0;
        if (idx >= segments.size()) idx = segments.size() - 1;
        return segments[idx].curvature(t);
    }
};

struct particle {
    HermiteSpline spline;
};

class Filter {
public:
    constexpr Filter(std::vector<std::vector<double>> waypoints) {
        if (waypoints[0].size()!= 3 || waypoints[1].size()!= 3) {
            throw std::invalid_argument("Invalid waypoint format for filter.");
        }
        // fit splines
        // break up splines further into smaller segments
    }
    constexpr double mutate_particle(particle& p) {
        /** @todo */
        // need to figure this out...
        /*
            break down the spline into smaller segments of hermite splines
            for each segment:
            - randomly perturb the waypoints
                ^-> for each random value:
                    take the distribution as a gaussian belle curve
                    use gradient descent to find the optimal spot. note that the spot will change, so potentially use adamw. get it working regularly then use gd
            - use automatic tangent fitting
            - recalculate the spline
        */
    }
    constexpr double calculate_fitness(const particle& p) {
        /** @todo */
        // need to figure this out...
        // likely use a cost function that takes speed between the waypoints
        // check to make sure that the particle is legal
    }
};