#include "include.cpp"

namespace cmn {
    class my_controller {
    public:
        pros::controller_analog_e_t steer_axis = pros::E_CONTROLLER_ANALOG_LEFT_Y;
        pros::controller_analog_e_t throttle_axis = pros::E_CONTROLLER_ANALOG_RIGHT_X;

        pros::controller_digital_e_t arm_pneumatic_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t wrist_pneumatic_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t intake_pneumatic_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t mogo_pneumatic_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t switch_controller_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t intake_motor_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t extake_motor_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t sort_color_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t load_arm_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t hold_ring_or_constant_scoring_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t score_held_ring_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t shift_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t chopper_mech = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t screen_index_button = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::controller_digital_e_t reverse = pros::E_CONTROLLER_DIGITAL_DOWN;
        pros::Controller *controller;
        bool is_master_controller = false;
        int current_slave_screen_index = 0;
    };

    class PID {
        public:
            double kP = 0.0f;
            double kD = 0.0f;
            double kI = 0.0f;
            double target = 0.0f;
            double position = 0.0f;
        private:
            double error;
            double x_1;
            double x_2;
            double integral;
        public:
            double run() {
                error = target - position;
                x_1 = x_2;
                x_2 = error;
                integral += error;
                return ((integral * kI) + (error * kP) + ((x_2 - x_1) * kD));
            }
    };

    class GUI {
    public:
        int current_view = 0;
        std::vector<pros::Motor> motor_list;
        int selected_auton = 0;
        std::vector<std::string> auton_name_list;

        void render () {
            clear_screen();
            switch (current_view){
                case 0:
                    auton_select_screen();
                    break;
                case 1:
                    print_motor_temprature_screen();
                    break;
                case 2:
                    print_motor_detail_screen();
                    break;
                case 3:
                    current_view = 0;
                    break;
                default:
                    break;
            }
            press();
        }

    private:
        pros::Color get_temp(pros::Motor& motor) {
            if(motor.get_temperature() < 55) {
                return pros::Color::green;
            }else if(motor.get_temperature() >= 55 && motor.get_temperature() < 60){
                return pros::Color::yellow;
            }else if(motor.get_temperature() >= 60 && motor.get_temperature() < 65){
                return pros::Color::red;
            } else if (motor.get_temperature() >= 65) {
                return pros::Color::dark_red;
            } else {
                return pros::Color::grey;
            }
        }

        void print_motor_temprature_screen() {
            pros::screen::set_pen(pros::Color::white_smoke);
            pros::screen::draw_line(360, 0, 360, 240);
            pros::screen::draw_line(360, 120, 480, 120);

            pros::screen::draw_circle(420, 60, 46);
            pros::screen::draw_circle(420, 180, 46);
            pros::screen::draw_circle(300, 60, 46);
            pros::screen::draw_circle(180, 60, 46);
            pros::screen::draw_circle(60, 60, 46);
            pros::screen::draw_circle(60, 180, 46);
            pros::screen::draw_circle(180, 180, 46);
            pros::screen::draw_circle(300, 180, 46);
            pros::screen::set_pen(get_temp(motor_list[6]));
            pros::screen::fill_circle(420, 60, 45);
            pros::screen::set_pen(get_temp(motor_list[7]));
            pros::screen::fill_circle(420, 180, 45);
            pros::screen::set_pen(get_temp(motor_list[2]));
            pros::screen::fill_circle(300, 60, 45);
            pros::screen::set_pen(get_temp(motor_list[5]));
            pros::screen::fill_circle(300, 180, 45);
            pros::screen::set_pen(get_temp(motor_list[1]));
            pros::screen::fill_circle(180, 60, 45);
            pros::screen::set_pen(get_temp(motor_list[4]));
            pros::screen::fill_circle(180, 180, 45);
            pros::screen::set_pen(get_temp(motor_list[0]));
            pros::screen::fill_circle(60, 60, 45);
            pros::screen::set_pen(get_temp(motor_list[3]));
            pros::screen::fill_circle(60, 180, 45);

            pros::screen::set_eraser(pros::Color::black);
            pros::screen::set_pen(pros::Color::white_smoke);
            pros::screen::print(pros::E_TEXT_SMALL, 17, 55, "L M 1 (%i)", motor_list[0].get_port());
            pros::screen::print(pros::E_TEXT_SMALL, 137, 55, "L M 2 (%i)", motor_list[1].get_port());
            pros::screen::print(pros::E_TEXT_SMALL, 257, 55, "L M 3 (%i)", motor_list[2].get_port());
            pros::screen::print(pros::E_TEXT_SMALL, 17, 175, "R M 1 (%i)", motor_list[3].get_port());
            pros::screen::print(pros::E_TEXT_SMALL, 137, 175, "R M 2 (%i)", motor_list[4].get_port());
            pros::screen::print(pros::E_TEXT_SMALL, 257, 175, "R M 3 (%i)", motor_list[5].get_port());
            pros::screen::print(pros::E_TEXT_SMALL, 377, 55, "Lower(%i)", motor_list[6].get_port());
            pros::screen::print(pros::E_TEXT_SMALL, 377, 175, "Upper(%i)", motor_list[7].get_port());
        }

        void clear_screen() {
            pros::screen::set_eraser(pros::Color::black);
            pros::screen::erase;
            pros::screen::erase_rect(0,0,480,240);
        }

        void print_motor_detail_screen () {
            pros::screen::set_pen(pros::Color::white_smoke);
            pros::screen::draw_line(360, 0, 360, 240);
            pros::screen::draw_line(240, 0, 240, 240);
            pros::screen::draw_line(120, 0, 120, 240);
            pros::screen::draw_line(0, 120, 480, 120);
            std::vector<std::string> stat_type {"Velocity ", "Temp ", "Torque ", "Power "};
            std::vector<std::string> unit {"RPM", "C", "nm", "W"};

            for(int i = 0; i < 4; ++i){
                for(int  j = 0; j < 2; ++j){
                    int velocity = motor_list[i + j].get_actual_velocity();
                    int temprature = motor_list[i + j].get_temperature();
                    int torque = std::roundf(motor_list[i + j].get_torque());
                    int power = motor_list[i + j].get_power();
                    std::vector<std::string> stat {std::to_string(velocity), std::to_string(temprature), std::to_string(torque), std::to_string(power)};
                    for(int k = 1; k <= 4; ++k) {
                        pros::screen::print(pros::E_TEXT_SMALL, (120 * i) + (8), (j * 120) + (k * 20), "%s%s%s", stat_type[k-1], stat[k-1], unit[k-1]);
                    }
                }
            }
        }

        std::vector<int> is_selected() {
            int x_1, y_1, x_2, y_2;
            if((selected_auton % 2) == 0) {
                y_1 = 10;
                y_2 = 95;
            } else {
                y_1 = 115;
                y_2 = 200;
            }
            x_1 = (((selected_auton - (selected_auton % 2)) / 2) * 120) + 10;
            x_2 = x_1 + 100;

            return {x_1, y_1, x_2, y_2};
        }

        void auton_select_screen() {
            clear_screen();
            
            pros::screen::set_pen(pros::Color::purple);
            pros::screen::fill_rect(is_selected()[0], is_selected()[1], is_selected()[2], is_selected()[3]);
            
            pros::screen::set_pen(pros::Color::light_blue);
            for(int i = 0; i < 4; ++i) {
                for(int j = 0; j < 2; ++j) {
                    pros::screen::fill_rect(((i * 120) + 20), ((j * 105) + 20), (((i + 1) * 120) - 20), (((j + 1) * 105) - 20));
                }
            }

            pros::screen::set_pen(pros::Color::white_smoke);
            pros::screen::draw_line(0, 211, 480, 211);
            pros::screen::print(pros::E_TEXT_MEDIUM_CENTER, 240, 225, "Next Display ->");

            for(int k = 0; k < 4; ++k) {
                for(int a = 0; a < 2; ++a) {
                    //pros::screen::print(pros::E_TEXT_SMALL, ((k * 120) + 10), ((a * 120) + 55), "%s", auton_name_list[(k * 2) + a]);
                }
            }
        }

        void press() {
            pros::screen_touch_status_s_t status = pros::screen::touch_status();
            if(status.touch_status == pros::E_TOUCH_PRESSED){
                if(!(current_view == 0)) {
                    ++current_view;
                } else if(status.y > 215) {
                    ++current_view;
                } else {
                    selected_auton = (std::floor(status.x / 120) * 2) + (std::floor(status.y / 105));
                }
                if(current_view == 3) {
                    current_view = 0;
                }
            }
        }
    };

    class Tube {
    public:
        double length = 0; //in inches
        double volume;
        double init() {
            volume = (std::pow((1.0f / 25.4), 2) * M_PI) * length; //in cubic inches
            return volume;
        }
    };

    class Piston {
    public:
        double stroke = 0; //in mm
        double volume;
        double init() {
            volume = (std::pow((5.0f / 25.4), 2) * M_PI) * (stroke / 25.4); //in cubic inches
            return volume;
        }
    };

    class Air_Tank {
    public:
        std::string type; // default new type
        double capacity;
        
        Air_Tank(std::string tank_type = "new") : type(tank_type) {
            if (type == "legacy") {
                capacity = LEGACY_CAPACITY;
            } else {
                capacity = NEW_CAPACITY;
            }
        }
    private:
        const double LEGACY_CAPACITY = 5.0; // example value
        const double NEW_CAPACITY = 9.154;
    };

    class Pneumatics_Component{
    public:
        std::vector<double> volumes;
        double total_component_volume; //in cubic inches

        void compute_system_volume () {
            total_component_volume = 0;
            for (int i = 0; i < volumes.size(); i++) {
                total_component_volume += volumes[i];
            }
        }
    };

    class Pneumatics_System {
    public:
        double air_pressure = 100.0f;
        double true_air_pressure = 110.0f;
        double system_volume;

        void system_init(std::vector<Pneumatics_Component> comp) {
            system_volume = 0;
            for (int i = 0; i < comp.size(); ++i) {
                system_volume += comp[i].total_component_volume;
            }
        }

        void cycled(Pneumatics_Component comp) {
            double new_volume = system_volume + comp.total_component_volume;
            true_air_pressure = true_air_pressure * (system_volume / new_volume);
            if (true_air_pressure < 100) {
                air_pressure = true_air_pressure;
            } else {
                air_pressure = 100;
            }
        }
    };

    // In the future, a controller lcd class for spontainious match stats
    class Controller_Print {
    public:

        std::string my_time_to_string(unsigned int time) {
        unsigned int minutes = time / 60;
        unsigned int seconds = time % 60;
        std::string minutes_str = std::to_string(minutes);
        std::string seconds_str = std::to_string(seconds);
        if (minutes < 10) {
            minutes_str = "0" + minutes_str;
        }
        if (seconds < 10) {
            seconds_str = "0" + seconds_str;
        }
        return minutes_str + ":" + seconds_str;
    }
    
        std::vector<pros::Motor> cont_drivetrain;
        unsigned int time;
        Pneumatics_System p_sys;
        float pressure_derivative, battery_derivative, temp_derivative;

        int get_pressure (Pneumatics_System p_sys) {
            return std::round(p_sys.true_air_pressure);
        }

        void init (my_controller& master, my_controller& slave) {
            master.controller->clear();
            slave.controller->clear();
            pd_1 = p_sys.true_air_pressure;
            bd_1 = pros::battery::get_capacity();
            td_1 = get_current_temp();
        }

        void print_master (my_controller master) {
            //master.controller->clear();
            master.controller->print(1, 8, "%s", my_time_to_string(time / 1000));
        }

        void print_slave (my_controller slave) {
            //slave.controller->clear();
            pd_2 = p_sys.true_air_pressure;
            bd_2 = pros::battery::get_capacity();
            pressure_derivative = (pd_2 - pd_1) / (time / 1000);
            battery_derivative = ((bd_2 - bd_1) / (time / 1000)) * 100;
            double current_temp = get_current_temp();
            td_2 = current_temp;
            temp_derivative = (td_2 - td_1) / (time / 1000);
            if(slave.controller->get_digital(slave.screen_index_button)) {
                slave.current_slave_screen_index++;
            }
            switch(slave.current_slave_screen_index) {
                case 0:
                    slave.controller->print(0, 5, "%i PSI", std::round(p_sys.true_air_pressure));
                    slave.controller->print(1, 0, "%f/s", pressure_derivative);
                    slave.controller->print(2, 5, "%s", my_time_to_string(-(pd_1 / 1000)/ pressure_derivative));
                    break;
                case 1:
                    slave.controller->print(0, 6, "%f%", pros::battery::get_capacity() * 100);
                    slave.controller->print(1, 0, "%f/s", battery_derivative);
                    slave.controller->print(2, 5, "%s", my_time_to_string(-((bd_1 * 100) / 1000) / battery_derivative));
                    break;
                case 2:
                    slave.controller->print(0, 6, "%iC", std::round(current_temp));
                    slave.controller->print(1, 0, "%f/s", temp_derivative);
                    slave.controller->print(2, 5, "%s", my_time_to_string(-(td_1 / 1000) / temp_derivative));
                    break;
                case 3:
                    slave.controller->print(1, 5, "%s", my_time_to_string(time / 1000));
                    break;
                case 4:
                    slave.current_slave_screen_index = 0;
                    break;
                default:
                    break;
            }
        }
    private:
        double pd_1, pd_2, bd_1, bd_2, td_1, td_2;

        double get_current_temp () {
            return ((cont_drivetrain[0].get_temperature()) + (cont_drivetrain[1].get_temperature()) + (cont_drivetrain[2].get_temperature()) + (cont_drivetrain[3].get_temperature(), cont_drivetrain[4].get_temperature()) + (cont_drivetrain[5].get_temperature()) / 6);
        }
    };
}