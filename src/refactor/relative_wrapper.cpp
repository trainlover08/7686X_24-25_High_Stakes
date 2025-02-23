#include "refactor/relative_wrapper.hpp"
#include "lemlib/util.hpp"
#include <cmath>

namespace lemlib {

    Mod_Chassis::Mod_Chassis (Drivetrain drivetrain, ControllerSettings linearSettings, ControllerSettings angularSettings, OdomSensors sensors, DriveCurve* throttleCurve, DriveCurve* steerCurve) : Chassis(drivetrain, linearSettings, angularSettings, sensors, throttleCurve, steerCurve) {}

    void Mod_Chassis::moveRelative (float distance, unsigned int timeout) {
        Pose pose = this->getPose();
        bool reverse;
        if (distance < 0.0) reverse = true; else reverse = false;
        float x = (sin(pose.theta) * fabs(distance)) * (!reverse * 2 - 1) + pose.x;
        float y = (cos(pose.theta) * fabs(distance)) * (!reverse * 2 - 1) + pose.y;
        this->moveToPose(x, y, pose.theta, timeout, {.forwards=!reverse}, 0);
    }

    void Mod_Chassis::turnRelative (float theta, unsigned int timeout, bool reverse) {
        float theta0 = this->getPose().theta;
        theta0 += reverse * 180.0;
        float theta1 = sanitizeAngle(theta + theta0, false);
        this->turnToHeading(theta1, timeout, {}, 0);
    }
};