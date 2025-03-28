#include "colorizer.h"
#include <iostream>
#include <cmath>

Colorizer::Colorizer() : x(0), y(0), speed(1), angle(0) {}
Colorizer::~Colorizer() {}

void Colorizer::set_speed(double new_speed) {
    speed = new_speed;
    std::cout << "Задана скорость робота" << speed << std::endl;
}

void Colorizer::move_forward(unsigned int time) {

    int L = speed * time;
    double angle_rad = angle * 3.14 / 180.0;
    x += L * cos(angle_rad);
    y += L * sin(angle_rad);
    std::cout << "Робот движется вперед. Новая позиция: (" << x << ", " << y << ")" << std::endl;
}

void Colorizer::move_backward(unsigned int time) {
    int L = speed * time;
    double angle_rad = angle * 3.14 / 180.0;
    x -= L * cos(angle_rad);
    y -= L * sin(angle_rad);
    std::cout << "Робот движется назад. Новая позиция: (" << x << ", " << y << ")" << std::endl;
}

void Colorizer::turn_right(double usr_angle) {
    angle -= usr_angle;
    std::cout << "Робот поворачивает направо." << std::endl;
}

void Colorizer::turn_left(double usr_angle) {
    angle += usr_angle;
    std::cout << "Робот поворачивает налево." << std::endl;
}

bool Colorizer::activate_painting() {
    std::cout << "Робот закрашивает графитти." << std::endl;
    return true;
}


// тестовый колорайзер (вывод результата в консоль)

Test_Colorizer::Test_Colorizer() : Colorizer() {}
Test_Colorizer::~Test_Colorizer() {}

void Test_Colorizer::test_set_speed(double new_speed) {
    Colorizer::set_speed(new_speed);
    std::cout << "[Тест] Задание скорости протестировано." << std::endl;

}


void Test_Colorizer::test_move_forward(unsigned int time) {
    Colorizer::move_forward(time);
    std::cout << "[Тест] Движение вперед протестировано." << std::endl;
}

void Test_Colorizer::test_move_backward(unsigned int time) {
    Colorizer::move_backward(time);
    std::cout << "[Тест] Движение назад протестировано." << std::endl;
}

void Test_Colorizer::test_turn_right(double usr_angle) {
    Colorizer::turn_right(usr_angle);
    std::cout << "[Тест] Поворот направо протестирован." << std::endl;
}

void Test_Colorizer::test_turn_left(double usr_angle) {
    Colorizer::turn_left(usr_angle);
    std::cout << "[Тест] Поворот налево протестирован." << std::endl;
}

bool Test_Colorizer::test_activate_painting() {
    Colorizer::activate_painting();
    
}