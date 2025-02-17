#include "refactor/relative_wrapper.hpp"
#include "lemlib/util.hpp"
#include <cmath>

namespace lemlib {

    void Mod_Chassis::moveRelative (float distance, unsigned int timeout, bool reverse, bool async) {
        Pose pose = this->getPose();
        float x = cos(pose.theta) * distance + pose.x;
        float y = sin(pose.theta) * distance + pose.y;
        this->moveToPose(x, y, pose.theta, timeout, {.forwards=!reverse}, async);
    }

    void Mod_Chassis::turnRelative (float theta, unsigned int timeout, bool reverse, bool async) {
        float theta0 = this->getPose().theta;
        theta0 += reverse * 180.0;
        float theta1 = sanitizeAngle(theta + theta0, false);
        this->turnToHeading(theta1, timeout, {}, async);
    }
};