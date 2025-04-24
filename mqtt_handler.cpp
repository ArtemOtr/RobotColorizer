#include "mqtt_handler.h"
#include <iostream>
#include <vector>
#include <sstream>

MQTT_Handler::MQTT_Handler(const char* id, const char* host, int port, Robot_Controller& ctrl) 
    : mosquittopp(id), controller(ctrl) {
    connect(host, port, 60);
    loop_start();
    subscribe(nullptr, command_topic.c_str());
}

MQTT_Handler::~MQTT_Handler() {
    loop_stop();
    disconnect();
}

void MQTT_Handler::on_connect(int rc) {
    if(rc == 0) {
        std::cout << "Connected to MQTT Broker!" << std::endl;
    } else {
        std::cerr << "Failed to connect, error code: " << rc << std::endl;
    }
}

void MQTT_Handler::on_message(const mosquitto_message* message) {
    std::string payload(static_cast<char*>(message->payload), message->payloadlen);
    std::string topic(message->topic); 
    
    std::cout << "Received command: " << payload << std::endl;
    parse_command(payload);
}

void MQTT_Handler::parse_command(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    try {
        if(cmd == "move_to") {
            int x, y;
            if(iss >> x >> y) {
                controller.move_to_graffiti({x, y});
                send_status("Робот приехал на координаты " + std::to_string(x) + "," + std::to_string(y));
            }
        }
        else if(cmd == "paint") {
            if(controller.paint_over_graffiti()) {
                send_status("Закравшивание завершено");
            }
        }
        else if(cmd == "return_home") {
            if(controller.return_to_robot_base()) {
                send_status("Робот возвращен домой");
            }
        }
        else if(cmd == "set_speed") {
            double speed;
            if(iss >> speed) {
                
                controller.set_speed(speed);
                send_status("Теперь скокрость робота " + std::to_string(speed));
            }
        }
        
    } catch(const std::exception& e) {
        send_status("Error: " + std::string(e.what()));
    }
}

void MQTT_Handler::send_status(const std::string& status) {
    publish(nullptr, status_topic.c_str(), status.size(), status.c_str());
}