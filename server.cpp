#include <string>
#include <vector>
#include <iostream>
#include "server.h"


Server::Server() {}
Server::~Server() {}

bool Server::detect_graffiti() {
    std::cout << "Поиск граффити..." << std::endl;
    return true; // заглушка
}

std::vector<int> Server::get_graffiti_location() {
    std::cout << "Определение координат граффити..." << std::endl;
    return { 10, 20 }; // заглушка
}