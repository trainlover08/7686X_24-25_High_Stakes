#pragma once

#include "../lemlib/chassis/chassis.hpp"

namespace lemlib {
    class Mod_Chassis : public Chassis {
    public:
        using Chassis::Chassis;

        Mod_Chassis (Drivetrain drivetrain, ControllerSettings linearSettings, 
        ControllerSettings angularSettings, OdomSensors sensors, 
        DriveCurve* throttleCurve = &defaultDriveCurve, 
        DriveCurve* steerCurve = &defaultDriveCurve);

        void moveRelative (float distance, unsigned int timeout, bool reverse=0, bool async=1);
        void turnRelative (float angle, unsigned int timeout, bool reverse=0, bool async=1);

        virtual ~Mod_Chassis() = default;
    };
};