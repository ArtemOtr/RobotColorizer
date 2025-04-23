#include "robot_controller.h" // ��� �������� colorizer.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    setlocale(LC_ALL, "Russian");

    Test_Colorizer robot;
    Test_Controller receiver;
    std::string input;

    std::cout << "��������� �������:\n"
        << "1. ���������� �������: left/right/forward/backward/set_speed <��������> \n"
        << "2. ������� ��������: move_to <x> <y>, paint, return_home\n"
        << "3. �����: exit\n"
        << "������: move_to 10 5\n"
        << "        left 90 forward 2\n";

    while (true) {
        std::cout << "\n������� �������: ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        try {
            if (command == "left" || command == "right" ||
                command == "forward" || command == "backward" ||
                command == "set_speed") {

                double value;
                if (!(iss >> value)) {
                    throw std::runtime_error("�� ������� �������� ��� ������� " + command);
                }

                if (command == "left") robot.test_turn_left(value);
                else if (command == "right") robot.test_turn_right(value);
                else if (command == "forward") robot.test_move_forward(static_cast<unsigned int>(value));
                else if (command == "backward") robot.test_move_backward(static_cast<unsigned int>(value));
                else if (command == "set_speed") robot.test_set_speed(value);
            }
            else if (command == "move_to") {
                int x, y;
                if (!(iss >> x >> y)) {
                    throw std::runtime_error("�� ������� ���������� x � y");
                }
                receiver.test_move_to_graffiti({ x, y });
            }
            else if (command == "paint") {
                receiver.test_paint_over_graffiti();
            }
            else if (command == "return_home") {
                receiver.test_return_to_robot_base();
            }
            else if (command == "get_pos") {
                std::vector<double> pos = robot.get_robot_place(); // ����� �������� ����
                std::cout << "������� �������: " << pos[0] << " " << pos[1] << std::endl;
            }
            else {
                throw std::runtime_error("����������� �������: " + command);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "������: " << e.what() << std::endl;
        }
    }

    return 0;
}