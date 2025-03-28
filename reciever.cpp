#include "commands_reciever.h"
#include <vector>
#include <iostream>


Commands_Reciever::Commands_Reciever() : server(), colorizer() {}  //инициализация объектов

Commands_Reciever::~Commands_Reciever() {
    //деструкторы server и colorizer вызовутся автоматически
}

std::vector<int> Commands_Reciever::receive_graffiti_location() {
    return server.get_graffiti_location();
}

bool Commands_Reciever::move_to_graffiti(const std::vector<int>& coordinates) {
    if (coordinates.size() < 2) return false;
    // Логика перемещения
    return true;
}

bool Commands_Reciever::paint_over_graffiti() {
    return colorizer.activate_painting();  
}

bool Commands_Reciever::return_to_robot_base() {
    
    return true;
}

Test_Reciever::Test_Reciever() {}

Test_Reciever::~Test_Reciever() {}

std::vector<int> Test_Reciever::test_receive_graffiti_location() {
    auto location = receive_graffiti_location();
    std::cout << "Test: Receiving graffiti location at (" 
              << location[0] << ", " << location[1] << ")\n";
    return location;
}

bool Test_Reciever::test_move_to_graffiti(const std::vector<int>& coordinates) {
    std::cout << "Test: Moving to coordinates (" 
              << coordinates[0] << ", " << coordinates[1] << ")\n";
    return move_to_graffiti(coordinates);
}

bool Test_Reciever::test_paint_over_graffiti() {
    std::cout << "Test: Painting over graffiti\n";
    return paint_over_graffiti();
}

bool Test_Reciever::test_return_to_robot_base() {
    std::cout << "Test: Returning to robot base\n";
    return return_to_robot_base();
}