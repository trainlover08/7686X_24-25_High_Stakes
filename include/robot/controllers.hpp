#include "main.h"

// class for handling the controllers
class Mapable_Controller {
public:
    Mapable_Controller(pros::Controller *controller) : controller(controller) {
        controller->clear();
    }

    void print_time (unsigned int time) {
        unsigned int minutes = time / 60000;
        unsigned int seconds = (time % 60000) / 1000;

        controller->print(1, 9, "%02d:%02d", minutes, seconds);
    }

    void print_current_auton () {
        std::string str = selector->get_auton()->name;

        // delay between printing lines to avoid overloading the controller and nto printing at all
        pros::delay(500);
        controller->set_text(0, 1, "Selected Auto:");
        pros::delay(500);
        controller->set_text(1, 0, str);
    }

    void print_field_position (double x, double y, double theta) {
        controller->print(0, 0, "x: %lf", x);
        pros::delay(250);
        controller->print(1, 0, "y: %lf", y);
        pros::delay(250);
        controller->print(2, 0, "0: %lf", theta);
    }

    void print_motor_temps (std::vector<pros::Motor*> motors) {
        for (int i = 0; i < motors.size(); ++i) {
            controller->print(i % 3, (i % 4) * 3, motors[i]->get_temperature() < 55 ? "G%i" : (motors[i]->get_temperature() >= 55) && (motors[i]->get_temperature() >= 60) ? "Y%i" : "R%i", motors[i]->get_temperature());
        }
    }

    class Button_Combo {
    public:
        // Pass the controller pointer and button vector during initialization
        Button_Combo(pros::Controller *controller, std::vector<pros::controller_digital_e_t> button_vector)
            : controller(controller), button_vector(button_vector) {}

        bool is_pressing() {
            for (size_t j = 0; j < button_vector.size(); j++) {
                if (!controller->get_digital(button_vector[j])) {
                    return false;
                }
            }
            return true;
        }

    private:
        pros::Controller *controller; // Local controller reference
        std::vector<pros::controller_digital_e_t> button_vector;
    };
    
    pros::Controller *controller;
    rd::Selector *selector;

private:

};