#include "neural_network.cpp"
#include "file_io.cpp"

#include <vector>
#include <string>
#include <cmath>

class WB_Local_Controller {
public:
    double x_position, y_position, heading;
    double velocity_left, velocity_right, voltage_left, voltage_right;
    // Try without first
    // std::vector<std::pair<double, int>> object_type_distance;
};

class WB_Global_Controller {
public:
    double global_time;
};