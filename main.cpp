#include "colorizer.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    Test_Colorizer robot;
    std::string input;

    std::cout << "Введите команду (left/right/forward/backward <значение>):" << std::endl;
    std::cout << "Пример: left 45 forward 10" << std::endl;
    std::cout << "Для выхода введите 'exit'." << std::endl;

    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }

        std::istringstream iss(input);
        std::string command;
        double value;

        while (iss >> command) {
            if (command == "left") {
                if (iss >> value) {
                    robot.test_turn_left(value);
                } else {
                    std::cout << "Ошибка: не указано значение для команды 'left'." << std::endl;
                }
            } else if (command == "right") {
                if (iss >> value) {
                    robot.test_turn_right(value);
                } else {
                    std::cout << "Ошибка: не указано значение для команды 'right'." << std::endl;
                }
            } else if (command == "forward") {
                if (iss >> value) {
                    robot.test_move_forward(static_cast<unsigned int>(value));
                } else {
                    std::cout << "Ошибка: не указано значение для команды 'forward'." << std::endl;
                }
            } else if (command == "backward") {
                if (iss >> value) {
                    robot.test_move_backward(static_cast<unsigned int>(value));
                } else {
                    std::cout << "Ошибка: не указано значение для команды 'backward'." << std::endl;
                }
               
            } else if (command == "set_speed") {
                if (iss >> value) {
                    robot.test_set_speed(static_cast<unsigned int>(value));
                } else {
                    std::cout << "Ошибка: не указано значение для команды 'backward'." << std::endl;
                }
               
            } else {
                std::cout << "Ошибка: неизвестная команда '" << command << "'." << std::endl;
            }
        }
    }

    return 0;
}