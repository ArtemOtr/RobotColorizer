#include "camera_processor.h"
#include <opencv2/imgproc.hpp>
#include <stdexcept>

CameraProcessor::CameraProcessor() {
    colorRanges = {
        {cv::Scalar(140, 50, 50), cv::Scalar(170, 255, 255)},  // Розовый
        {cv::Scalar(100, 150, 0), cv::Scalar(140, 255, 255)},  // Синий
        {cv::Scalar(35, 100, 100), cv::Scalar(85, 255, 255)}  // Зелёный
    };
}

CameraProcessor::~CameraProcessor() = default;

std::vector<double> CameraProcessor::processFrameWithMask(const cv::Mat& frame) {
    try {
        if(frame.empty()) throw std::runtime_error("Empty frame");

        // 1. Получаем цветовую маску
        cv::Mat mask = getColorMask(frame);
        
        // 2. Находим контуры
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // 3. Фильтрация контуров по площади
        std::vector<cv::Rect> boxes;
        for(const auto& contour : contours) {
            if(cv::contourArea(contour) > min_contour_area) {
                boxes.emplace_back(cv::boundingRect(contour));
            }
        }

        // 4. Возвращаем координаты самого большого объекта
        if(!boxes.empty()) {
            cv::Rect largest = *std::max_element(boxes.begin(), boxes.end(),
                [](const cv::Rect& a, const cv::Rect& b) {
                    return a.area() < b.area();
                });
            
            // Нормализованные координаты центра
            double x = (largest.x + largest.width/2.0) / frame.cols;
            double y = (largest.y + largest.height/2.0) / frame.rows;
            return {x, y};
        }
        return {}; // Не найдено объектов
    }
    catch(const std::exception& e) {
        std::cerr << "Mask processing error: " << e.what() << std::endl;
        return {};
    }
}

cv::Mat CameraProcessor::applyColorMasks(const cv::Mat& hsvFrame) {
    cv::Mat combinedMask = cv::Mat::zeros(hsvFrame.size(), CV_8UC1);
    for (const auto& range : colorRanges) {
        cv::Mat mask;
        cv::inRange(hsvFrame, range.first, range.second, mask);
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::bitwise_or(combinedMask, mask, combinedMask);
    }
    return combinedMask;
}

cv::Mat CameraProcessor::getColorMask(const cv::Mat& frame) {
    if(frame.empty()) {
        throw std::runtime_error("Empty frame in getColorMask");
    }

    cv::Mat hsvFrame;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    return applyColorMasks(hsvFrame);
}