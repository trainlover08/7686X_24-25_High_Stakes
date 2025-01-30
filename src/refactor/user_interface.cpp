#include "user_interface.hpp"

Mapable_Controller::Mapable_Controller (pros::Controller* controller) {
      controller->this;
}

Mapable_Controller::Button_Combo::Button_Combo (std::vector<pros::controller_digital_e_t> button_vector) {
      button_vector->this;
}

bool Mapable_Controller::Button_Combo::is_pressing () {
    for (int i = 0; i < button_vector.size(); ++i) {
        if (!button_vector[i].is_pressing()) {
            return 0;
        }
        return 1;
    }
}