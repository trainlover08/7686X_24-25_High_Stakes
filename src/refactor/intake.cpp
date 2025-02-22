#include "refactor/intake.hpp"
#include "pros/adi.hpp"
#include <cmath>

Intake::Intake(pros::Motor* lower_intake, pros::Motor* upper_intake, pros::adi::Pneumatics* raiser, Color_Sort* lower_sort, pros::Rotation* rotation) {
     this->lower_intake = lower_intake;
     this->upper_intake = upper_intake;
     this->lower_sort = lower_sort;
     this->rotation = rotation;
     this->raiser = raiser;
     for (int i = 0; i < hook_poses.size(); ++i) {
          hook_poses[i] = (this->chain_length / hook_poses.size()) * i;
          hook_poses[i] += this->convert_angle_to_length(this->rotation->get_position());
     }
     this->pid = new lemlib::PID (this->kP, this->kI, this->kD);
}

Intake::Intake (pros::Motor* lower_intake, pros::Motor* upper_intake, pros::adi::Pneumatics* raiser) {
     this->lower_intake = lower_intake;
     this->upper_intake = upper_intake;
     this->raiser = raiser;
     this->lower_sort = nullptr;
}

double Intake::convert_angle_to_length (double angle) {
     // centigrees to degrees
     angle /= 100;
     // multiple by circumference
     double distance;
     distance = (angle/360) * (diameter * M_PI);
     // make sure length is always sanitized
     distance = distance > chain_length ? distance - chain_length : distance;
     distance = 0.0f > distance ? distance : distance + chain_length;
     return distance;
}

void Intake::move () {
     this->lower_intake->move(12000);
     this->upper_intake->move(12000);
}

void Intake::move (bool reverse) {
     if (reverse) {
          this->lower_intake->move(-12000);
          this->upper_intake->move(-12000);
     } else {Intake::move();}
}

void Intake::move (bool reverse, double percentage) {
     reverse = 2 * reverse - 1;
     percentage *= 12000.0 / 100.0;
     percentage *= reverse;
     this->lower_intake->move(percentage);
     this->upper_intake->move(percentage);
}

void Intake::move_lower () {
     this->lower_intake->move(12000);
}

void Intake::move_lower (double percentage) {
     this->lower_intake->move((percentage * 12000.0) / 100.0);
}

void Intake::move_lower (double percentage, bool reverse) {
     percentage *= reverse * 2 - 1;
     this->move_lower(percentage);
}

void Intake::move_upper () {
     this->upper_intake->move(12000);
}

void Intake::move_upper (double percentage) {
     this->upper_intake->move((percentage * 12000.0) / 100.0);
}

void Intake::move_upper (double percentage, bool reverse) {
     percentage *= reverse * 2 - 1;
     this->move_upper(percentage);
}

void Intake::stop () {
     this->upper_intake->move(0);
     this->lower_intake->move(0);
}

void Intake::stop_lower () {
     this->lower_intake->move(0);
}

void Intake::stop_upper () {
     this->upper_intake->move(0);
}

pros::Color Intake::get_color () {
     return this->lower_sort->get_color();
}

bool Intake::object_detected () {
     return this->lower_sort->object_detected();
}

pros::Color Intake::object_color () {
     pros::Color color= this->lower_sort->object_color();
     if (color != pros::Color::gray) {
          this->last_color = color;
     }
     return color;
}

void Intake::update_poses () {
     double default_pose = this->convert_angle_to_length(this->rotation->get_position());
     for (int i = 0; i < this->hook_poses.size(); ++i) {
          this->hook_poses[i] = default_pose + (this->chain_length / this->hook_poses.size()) * i;
     }
}

void Intake::nearest_hook_to_pose (double pose, bool reverse) {
     while (1) {
          this->update_poses();
          std::vector<double> error_vec = this->hook_poses;
          double error;
          double lerror;
          for (int i = 0; i < this->hook_poses.size(); ++i) {
               if (reverse) {
                    lerror = pose - this->hook_poses[i];
                    while (lerror < 0.0) lerror+=this->chain_length;
                    error_vec[i] = lerror;
               } else {
                    lerror = this->hook_poses[i] - pose;
                    while (lerror > this->chain_length) lerror -= this->chain_length;
                    error_vec[i] = lerror;
               }
          }
          std::sort(error_vec.begin(), error_vec.end());
          error = error_vec[0];
          error *= (!reverse * 2) - 1;
          double output = pid->update(error);
          this->upper_intake->move_velocity(output);
          if (0.5 > std::abs(error)) break;
          pros::delay(10);
     }
     this->stop_upper();
}

void Intake::move_hook_to_ready () {
     this->nearest_hook_to_pose(ready_pose);
}

void Intake::pickup_and_hold () {
     this->nearest_hook_to_pose(ready_pose + 3.5);
     this->nearest_hook_to_pose(ready_pose);
}

void Intake::load_lb () {
     this->pickup_and_hold();
     this->nearest_hook_to_pose(load_lb_pose);
     this->nearest_hook_to_pose(lb_backup_pose, true);
     // follow this motion with a move to ready
}

void Intake::score_ring () {
     this->move_hook_to_ready();
     this->upper_intake->move_relative(360.0 * ((this->chain_length * (3.0 / 4.0)) / (M_PI * this->diameter)), 100); // assuming units are degrees
     this->move_hook_to_ready();
}

void Intake::sort_ring () {
     this->pickup_and_hold();
     this->nearest_hook_to_pose(sort_stop_pose);
     this->upper_intake->move_velocity(0);
     pros::delay(250);
     this->move_hook_to_ready();
}

void Intake::raise_intake (bool extend) {
     this->raiser->set_value(extend);
}