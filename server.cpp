#include "server.h"
#include "camera_processor.h"
#include <opencv2/highgui.hpp>

Server::Server(const std::string& camera_url, 
               const cv::Scalar& lower_color,
               const cv::Scalar& upper_color) 
    : lower_color_(lower_color), upper_color_(upper_color) {
    
    if (camera_url.find("http") != std::string::npos || 
        camera_url.find("rtsp") != std::string::npos) {
        camera_.open(camera_url);
    } else {
        camera_.open(std::stoi(camera_url));
    }
    
    if (!camera_.isOpened()) {
        throw std::runtime_error("Не удалось открыть камеру: " + camera_url);
    }
    
    camera_.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    camera_.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    camera_.set(cv::CAP_PROP_FPS, 30);
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

cv::Mat Server::apply_color_mask(const cv::Mat& hsv_frame) {
    cv::Mat mask;
    cv::inRange(hsv_frame, lower_color_, upper_color_, mask);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    return mask;
}

std::vector<double> Server::get_graffiti_location_with_homography(bool show_windows) {
    cv::Mat frame = process_frame();
    if(frame.empty()) return {};
    
    CameraProcessor processor;
    processor.setColorRange(lower_color_, upper_color_);
    auto result = processor.processFrameWithHomography(frame);
    
    if(show_windows) {
        cv::Mat display_frame = frame.clone();
        if(!result.empty()) {
            cv::Point center(frame.cols/2, frame.rows/2);
            cv::Point target(result[0] * frame.cols, result[1] * frame.rows);
            cv::line(display_frame, center, target, cv::Scalar(0, 255, 0), 2);
            cv::circle(display_frame, target, 10, cv::Scalar(0, 0, 255), 2);
        }
        cv::imshow("Camera View", display_frame);
        cv::waitKey(1);
    }
    
    return result;
}

bool Server::is_graffiti_painted() {
    cv::Mat frame = process_frame();
    cv::Mat hsv_frame, mask;
    cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
    mask = apply_color_mask(hsv_frame);
    return cv::countNonZero(mask) < min_pixels_;
}

void Server::set_color_range(const cv::Scalar& lower, const cv::Scalar& upper) {
    lower_color_ = lower;
    upper_color_ = upper;
}

void Server::set_min_pixels(int min_pixels) {
    min_pixels_ = min_pixels;
}