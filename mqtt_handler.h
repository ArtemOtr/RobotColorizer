#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <mosquittopp.h>
#include <string>
#include "robot_controller.h"

class MQTT_Handler : public mosqpp::mosquittopp {
public:
    MQTT_Handler(const char* id, const char* host, int port, Robot_Controller& controller);
    ~MQTT_Handler();

    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message* message) override;
    void send_status(const std::string& status);

private:
    Robot_Controller& controller;
    const std::string command_topic = "robot/commands";
    const std::string status_topic = "robot/status";

    void parse_command(const std::string& command);
};
#endif // MQTT_HANDLER_H