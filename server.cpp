#include "server.h"
#include "camera_processor.h"
#include <opencv2/highgui.hpp>

Server::Server(const std::string& camera_url)
{
    if (camera_url.find("http") != std::string::npos || 
        camera_url.find("rtsp") != std::string::npos) {
        camera_.open(camera_url);
    } else {
        camera_.open(std::stoi(camera_url), cv::CAP_V4L2);
    }
    
    if (!camera_.isOpened()) {
        throw std::runtime_error("Не удалось открыть камеру: " + camera_url);
    }
    
    camera_.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    camera_.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    camera_.set(cv::CAP_PROP_FPS, 30);
    
    processor_ = std::make_unique<CameraProcessor>();
}

Server::~Server() {
    if (camera_.isOpened()) {
        camera_.release();
    }
}

cv::Mat Server::process_frame() {
    cv::Mat frame;
    camera_ >> frame;
    if (frame.empty()) {
        throw std::runtime_error("Пустой кадр с камеры");
    }
    return frame;
}

std::vector<double> Server::get_graffiti_location_with_homography(bool show_windows) {
    cv::Mat frame = process_frame();
    if(frame.empty()) return {};

    auto result = processor_->processFrameWithMask(frame); // Измененный вызов
    
    if(show_windows) {
        cv::Mat display_frame = frame.clone();
        if(!result.empty()) {
            // Преобразование нормализованных координат в пиксельные
            int px_x = static_cast<int>(result[0] * display_frame.cols);
            int px_y = static_cast<int>(result[1] * display_frame.rows);
            
            cv::Point center(display_frame.cols/2, display_frame.rows/2);
            cv::Point target(px_x, px_y);
            
            cv::line(display_frame, center, target, cv::Scalar(0,255,0), 2);
            cv::circle(display_frame, target, 10, cv::Scalar(0,0,255), 2);
        }
        cv::imshow("Camera View", display_frame);
        cv::waitKey(1);
    }
    return result;
}

bool Server::is_graffiti_painted() {
    cv::Mat frame = process_frame();
    cv::Mat hsv_frame;
    cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);

    // Общая маска всех цветовых областей
    cv::Mat mask = processor_->applyColorMasks(hsv_frame);
    return cv::countNonZero(mask) < min_pixels_;
}

void Server::set_min_pixels(int min_pixels) {
    min_pixels_ = min_pixels;
}