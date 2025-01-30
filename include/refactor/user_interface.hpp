#pragma once

#include "pros/misc.hpp"
#include <vector>

class Mapable_Controller {
public:
     Mapable_Controller(pros::Controller* controller);
     pros::Controller* controller;
     class Button_Combo {
     public:
          Button_Combo(pros::Controller* controller, std::vector<pros::controller_digital_e_t> button_vector);
          bool is_pressing();
     private:
          std::vector<pros::controller_digital_e_t> button_vector;
          pros::Controller* controller;
     };
};