#include "robot_controller.h"
#include <cmath>
#include <thread>
#include <chrono>
#include <opencv2/highgui.hpp>


Robot_Controller::Robot_Controller() : colorizer() {}

Robot_Controller::~Robot_Controller() {
    //деструкторы server и colorizer вызовутся автоматически
}

std::vector<double> Robot_Controller::receive_graffiti_location() {
    return server.get_graffiti_location_with_homography(false);
}

bool Robot_Controller::move_to_graffiti(const std::vector<double>& target) {
    if (target.size() < 2) return false;

    const double eps = 0.5; // Точность достижения
    const int max_steps = 100; // Ограничение шагов
    int steps = 0;

    while (steps++ < max_steps) {
        std::vector<double> current_pos = colorizer.get_robot_place();
        double dx = target[0] - current_pos[0];
        double dy = target[1] - current_pos[1];

        // Вывод текущей позиции
        std::cout << "Текущая позиция: [" << current_pos[0] << ", " << current_pos[1] << "]\n";
        std::cout << "Цель: [" << target[0] << ", " << target[1] << "]\n";

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

std::vector<double> Test_Controller::test_receive_graffiti_location() {
    auto location = receive_graffiti_location();
    std::cout << "Test: Receiving graffiti location at ("
              << location[0] << ", " << location[1] << ")\n";
    return location;
}

bool Test_Controller::test_move_to_graffiti(const std::vector<double>& coordinates) {
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

bool Robot_Controller::auto_approach_graffiti(bool show_debug) {
    // 1. Получаем координаты граффити с визуализацией
    auto target = server.get_graffiti_location_with_homography(show_debug);
    if(target.empty()) {
        if(show_debug) {
            std::cout << "Граффити не найдено!" << std::endl;
        }
        return false;
    }
    
    double target_x = target[0];
    double target_y = target[1];
    
    // 2. Получаем текущую позицию
    auto pos = colorizer.get_robot_place();
    double x = pos[0], y = pos[1], angle = pos[2];
    
    // 3. Вычисляем вектор к цели
    double dx = target_x - x;
    double dy = target_y - y;
    double distance = sqrt(dx*dx + dy*dy);
    
    // 4. Вычисляем требуемый угол
    double target_angle = atan2(dy, dx) * 180.0 / M_PI;
    double angle_diff = target_angle - angle;
    
    // Нормализуем угол (-180..180)
    while(angle_diff > 180) angle_diff -= 360;
    while(angle_diff < -180) angle_diff += 360;
    
    // 5. Поворачиваем робота
    if(abs(angle_diff) > 5) { // 5° - точность
        if(angle_diff > 0) {
            colorizer.turn_left(angle_diff);
        } else {
            colorizer.turn_right(-angle_diff);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    // 6. Движемся к цели
    if(distance > 0.05) { // 5 см - точность
        colorizer.move_forward(static_cast<int>(distance * 100)); // м -> см
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    if(show_debug) {
        std::cout << "Достигнута позиция: " << target_x << ", " << target_y << std::endl;
    }
    
    return true;
}

bool Test_Controller::test_auto_approach_graffiti(bool show_debug) {
    std::cout << "[Тест] Автоматическое наведение на граффити\n";
    return auto_approach_graffiti(show_debug);
}