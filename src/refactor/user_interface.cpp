#include "refactor/user_interface.hpp"
#include "pros/misc.h"

Mapable_Controller::Mapable_Controller (pros::Controller* controller) {
    this->controller = controller;
}

Mapable_Controller::Button_Combo::Button_Combo (pros::Controller* controller, std::vector<pros::controller_digital_e_t> button_vector) {
    this->button_vector = button_vector;
    this->controller = controller;
}

bool Mapable_Controller::Button_Combo::is_pressing () {
    for (int i = 0; i < button_vector.size(); ++i) {
        if (!controller->get_digital(button_vector[i])) {
            return 0;
        }
        return 1;
    }
}