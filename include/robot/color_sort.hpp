#include "main.h"

class Color_Sort {
public:
    Color_Sort(pros::Optical *optical, pros::Distance *distance, pros::adi::Pneumatics *piston, unsigned int rotations, pros::Motor *motor) : optical(optical), distance(distance), piston(piston), rotations(rotations), motor(motor) {}

    void sort(pros::Color color) {
        // checks to see if the color if an object is close to the sensor
        if (distance->get_distance() < 4) {
            // conditional to check if the passed color matches
            if (color == pros::Color::red) {
                // checks if the color is more blue than red
                if (optical->get_rgb().red < optical->get_rgb().blue) {
                    fling();
                }
            } else if (color == pros::Color::blue) {
                if (optical->get_rgb().red > optical->get_rgb().blue) {
                    fling();
                }
            }
        }
    }

    bool object_detected () {
        return distance->get_distance() < 4;
    }

    pros::Color get_color () {
        // detects the color in a similar way as before
        if (distance->get_distance()) {
            if (optical->get_rgb().red < optical->get_rgb().blue) {
                return pros::Color::blue;
            } else if (optical->get_rgb().red > optical->get_rgb().blue) {
                return pros::Color::red;
            }
        }
        return pros::Color::gray;
    }

    unsigned int rotations;
    bool enable = 1;
private:
    pros::Optical *optical;
    pros::Distance *distance;
    pros::adi::Pneumatics *piston;
    pros::Mutex sort_mutex;
    pros::Motor *motor;

    // if called, it ejects the ring
    void fling () {
        // checks to see if color sorting is on
        if (enable) {
            // spin to the correct amount before ejecting
            double current_position = motor->get_position();
            while (current_position < rotations) {
                pros::delay(10);
            }
            sort_mutex.take();
            piston->extend();
            sort_mutex.give();
            pros::delay(100);
            sort_mutex.take();
            piston->retract();
            sort_mutex.give();
        }
    }
};