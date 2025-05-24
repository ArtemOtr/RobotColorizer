#include "robot_controller.h"
#include "mqtt_handler.h"
#include "camera_processor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main() {
    setlocale(LC_ALL, "Russian");

    // Инициализация окон для визуализации
    cv::namedWindow("Camera View", cv::WINDOW_NORMAL);
    cv::namedWindow("Color Mask", cv::WINDOW_NORMAL);
    cv::namedWindow("Debug Info", cv::WINDOW_NORMAL);
    cv::resizeWindow("Camera View", 640, 480);
    cv::resizeWindow("Color Mask", 320, 240);
    cv::resizeWindow("Debug Info", 640, 100);

    // Инициализация камеры
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Ошибка: Не удалось открыть камеру." << std::endl;
        return -1;
    }

    Test_Colorizer robot;
    Test_Controller controller;
    CameraProcessor processor;
    MQTT_Handler mqtt("robot_client", "localhost", 1883, controller);

    std::cout << "Доступные команды:\n"
              << "1. Управление роботом: left/right/forward/backward/set_speed <значение>\n"
              << "2. Команды ресивера: move_to <x> <y>, paint, return_home, auto_paint\n"
              << "3. Выход: exit\n"
              << "Пример: move_to 10 5\n"
              << "        left 90 forward 2\n";

    std::string input;
    bool running = true;

    while (running) {
        // Проверка окон
        if (cv::getWindowProperty("Camera View", cv::WND_PROP_VISIBLE) < 1 ||
            cv::getWindowProperty("Color Mask", cv::WND_PROP_VISIBLE) < 1) {
            break;
        }

        // Захват кадра
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Ошибка: Кадр не получен с камеры." << std::endl;
            break;
        }

        // Обработка маски
        cv::Mat mask = processor.getColorMask(frame);
        cv::Mat maskedView;
        cv::bitwise_and(frame, frame, maskedView, mask);

        // Отображение видео
        cv::imshow("Camera View", frame);
        cv::imshow("Color Mask", maskedView);

        // Обновление отладочной информации
        std::vector<double> pos = robot.get_robot_place();
        cv::Mat debug_img(100, 640, CV_8UC3, cv::Scalar(0));
        std::string pos_text = "Position: " + std::to_string(pos[0]) + ", " + std::to_string(pos[1]);
        cv::putText(debug_img, pos_text, cv::Point(10, 30), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 1);
        cv::imshow("Debug Info", debug_img);

        // Проверка пользовательского ввода
        std::cout << "\nВведите команду: ";
        std::getline(std::cin, input);

        if (input == "exit") break;

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
                double x, y;
                if (!(iss >> x >> y)) {
                    throw std::runtime_error("Не указаны координаты x и y");
                }
                controller.test_move_to_graffiti({x, y});
            }
            else if (command == "paint") {
                controller.test_paint_over_graffiti();
            }
            else if (command == "return_home") {
                controller.test_return_to_robot_base();
            }
            else if (command == "auto_paint") {
                if (controller.auto_approach_graffiti()) {
                    controller.test_paint_over_graffiti();
                }
            }
            else if (command == "get_pos") {
                std::cout << "Текущая позиция: " << pos[0] << " " << pos[1] << std::endl;
            }
            else {
                throw std::runtime_error("Неизвестная команда: " + command);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }

        cv::waitKey(50);
    }

    cv::destroyAllWindows();
    return 0;
}