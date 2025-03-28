#include "refactor/intake.hpp"
#include "pros/adi.hpp"
#include <cmath>
#include "connor.h"

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

Intake::Intake (pros::Motor* lower_intake, pros::Motor* upper_intake, pros::adi::Pneumatics* raiser, pros::Optical* opt, pros::Distance* dist) {
     this->lower_intake = lower_intake;
     this->upper_intake = upper_intake;
     this->raiser = raiser;
     this->lower_sort = nullptr;
     this->opt = opt;
     this->opt->set_led_pwm(100);
     pros::Controller master(pros::E_CONTROLLER_MASTER);
     this->master = &master;
     this->dist = dist;
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

void Intake::color_sort (pros::Color color) {
     pros::Color o_col;
     if (this->opt->get_proximity() > 200) {
          if (this->opt->get_rgb().red > this->opt->get_rgb().blue) o_col = pros::Color::red; else o_col = pros::Color::blue;
          if (o_col == color) {
               this->upper_intake->move_velocity(600);
               int count = 0;
               while (this->dist->get_distance() > SENSE_DIST) {
                    this->upper_intake->move_velocity(600);
                    pros::delay(10);
                    count++;
                    if (count > 1000) break;
                    //if (master->get_digital(l1)) return;
               }
               this->upper_intake->move_velocity(600);
               count = 0;
               while (this->dist->get_distance() < SENSE_DIST) {
                    this->upper_intake->move_velocity(600);
                    pros::delay(10);
                    count++;
                    if (count > 500) break;
               }
               this->upper_intake->move_velocity(600);
               this->upper_intake->tare_position();
               count = 0;
               while (this->upper_intake->get_position() < SORT_MACRO) {
                    count++;
                    this->upper_intake->move_velocity(600);
                    pros::delay(10);
                    if (count > 500) break;
                    //if (master->get_digital(l1)) return;
               }
               this->upper_intake->move_velocity(0);
               count = 0;
               while (this->upper_intake->get_actual_velocity() > 1) {
                    pros::delay(50);
                    count++;
                    if (count > 500) break;
                    //if (master->get_digital(l1)) return;
               }
               return;
          }
     }
}

void Intake::anti_jam () {
     double v1, v2 = 0.0;
     pros::Mutex m;
     while (1) {
          v1 = v2;
          m.take();
          v2 = this->upper_intake->get_actual_velocity();
          m.give();
          if (1) {
               m.take();
               this->move_upper(-100);
               pros::delay(900);
               m.give();
          }
          pros::delay(10);
     }
}