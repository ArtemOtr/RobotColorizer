#include "robot_controller.h"
#include <vector>
#include <iostream>
#include <cmath> 
#include <thread>
#include <chrono>

Robot_Controller::Robot_Controller() : server(), colorizer() {}  //инициализация объектов

Robot_Controller::~Robot_Controller() {
    //деструкторы server и colorizer вызовутся автоматически
}

std::vector<int> Robot_Controller::receive_graffiti_location() {
    return server.get_graffiti_location();
}

bool Robot_Controller::move_to_graffiti(const std::vector<int>& target_coords) {
    if (target_coords.size() < 2) return false;

    const double eps = 0.5; // Точность достижения
    const int max_steps = 100; // Ограничение шагов
    int steps = 0;

    while (steps++ < max_steps) {
        std::vector<double> current_pos = colorizer.get_robot_place();
        double dx = target_coords[0] - current_pos[0];
        double dy = target_coords[1] - current_pos[1];

        // Вывод текущей позиции
        std::cout << "Текущая позиция: [" << current_pos[0] << ", " << current_pos[1] << "]\n";
        std::cout << "Цель: [" << target_coords[0] << ", " << target_coords[1] << "]\n";

        // Проверка достижения цели
        if (std::abs(dx) < eps && std::abs(dy) < eps) {
            std::cout << "Цель достигнута!\n";
            return true;
        }

        // Определяем главное направление (X или Y)
        bool move_x = std::abs(dx) > std::abs(dy);

        // Пытаемся двигаться по главному направлению
        if (move_x) {
            if (dx > eps) {
                colorizer.move_forward(1); // Вперед по X
            }
            else if (dx < -eps) {
                colorizer.move_backward(1); // Назад по X
            }
        }
        else {
            if (dy > eps) {
                colorizer.turn_left(90);
                colorizer.move_forward(1); // Вперед по Y
                colorizer.turn_right(90);
            }
            else if (dy < -eps) {
                colorizer.turn_left(90);
                colorizer.move_backward(1); // Назад по Y
                colorizer.turn_right(90);
            }
        }

        // Проверка стены (заглушка)
        if (steps % 500000 == 0) { // Каждые 5 шагов "стена"
            std::cout << "Обнаружена стена! Меняю направление...\n";
            colorizer.turn_right(90); // Поворот при препятствии
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "Превышено максимальное количество шагов!\n";
    return false;
}


// Простая функция задержки
void sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

bool Robot_Controller::paint_over_graffiti() {
    return colorizer.activate_painting();
}

bool Robot_Controller::return_to_robot_base() {

    return true;
}

Test_Controller::Test_Controller() {}

Test_Controller::~Test_Controller() {}

std::vector<int> Test_Controller::test_receive_graffiti_location() {
    auto location = receive_graffiti_location();
    std::cout << "Test: Receiving graffiti location at ("
        << location[0] << ", " << location[1] << ")\n";
    return location;
}

bool Test_Controller::test_move_to_graffiti(const std::vector<int>& coordinates) {
    std::cout << "Test: Moving to coordinates ("
        << coordinates[0] << ", " << coordinates[1] << ")\n";
    return move_to_graffiti(coordinates);
}

bool Test_Controller::test_paint_over_graffiti() {
    std::cout << "Test: Painting over graffiti\n";
    return paint_over_graffiti();
}

bool Test_Controller::test_return_to_robot_base() {
    std::cout << "Test: Returning to robot base\n";
    return return_to_robot_base();
}