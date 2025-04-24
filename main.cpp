#include "robot_controller.h" // Уже включает colorizer.h
#include "mqtt_handler.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    setlocale(LC_ALL, "Russian");

    Test_Colorizer robot;
    Test_Controller controller;
    MQTT_Handler mqtt("robot_client", "localhost", 1883, controller);
    std::string input;

    std::cout << "Доступные команды:\n"
        << "1. Управление роботом: left/right/forward/backward/set_speed <значение> \n"
        << "2. Команды ресивера: move_to <x> <y>, paint, return_home\n"
        << "3. Выход: exit\n"
        << "Пример: move_to 10 5\n"
        << "        left 90 forward 2\n";

    while (true) {
        std::cout << "\nВведите команду: ";
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
                    throw std::runtime_error("Не указано значение для команды " + command);
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
                    throw std::runtime_error("Не указаны координаты x и y");
                }
                controller.test_move_to_graffiti({ x, y });
            }
            else if (command == "paint") {
                controller.test_paint_over_graffiti();
            }
            else if (command == "return_home") {
                controller.test_return_to_robot_base();
            }
            else if (command == "get_pos") {
                std::vector<double> pos = robot.get_robot_place(); // ����� �������� ����
                std::cout << "Текущая позиция: " << pos[0] << " " << pos[1] << std::endl;
            }
            else {
                throw std::runtime_error("Неизвестная команда: " + command);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}