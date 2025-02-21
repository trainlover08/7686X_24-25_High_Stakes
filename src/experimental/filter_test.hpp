#include "MCL/refactor/particle_filter.hpp"
#include "refactor/constructors.hpp"

/*pros::Distance mcl_distance(0);
MCL::sensor sensor ({.d=&mcl_distance, .x=4, .y=6, .theta=0});
MCL mcl(&chassis_pose, {sensor});

void localize () {
    while (1) {
        chassis_pose = chassis.getPose();
        lemlib::Pose* predicted_pose = mcl.update();
        pros::delay(10);
    }
}*/