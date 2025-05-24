#include "camera_processor.h"
#include <opencv2/calib3d.hpp>

CameraProcessor::CameraProcessor() 
    : lowerColor(0, 120, 70), upperColor(10, 255, 255) {}

CameraProcessor::~CameraProcessor() = default;

void CameraProcessor::setColorRange(const cv::Scalar& lower, const cv::Scalar& upper) {
    lowerColor = lower;
    upperColor = upper;
}

cv::Mat CameraProcessor::applyColorMask(const cv::Mat& hsvFrame) {
    cv::Mat mask;
    cv::inRange(hsvFrame, lowerColor, upperColor, mask);
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1,-1), 2);
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1,-1), 2);
    return mask;
}

cv::Mat CameraProcessor::getColorMask(const cv::Mat& frame) {
    cv::Mat hsvFrame;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    return applyColorMask(hsvFrame);
}

std::vector<cv::Point> CameraProcessor::findContours(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    if(contours.empty()) return {};
    
    auto largest = std::max_element(contours.begin(), contours.end(),
        [](const auto& a, const auto& b) {
            return cv::contourArea(a) < cv::contourArea(b);
        });
    
    return *largest;
}

std::vector<double> CameraProcessor::processFrameWithHomography(const cv::Mat& frame) {
    cv::Mat hsvFrame;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    cv::Mat mask = applyColorMask(hsvFrame);
    auto contour = findContours(mask);
    if(contour.empty()) return {};
    
    cv::Rect bounds = cv::boundingRect(contour);
    
    std::vector<cv::Point2f> imagePoints = {
        cv::Point2f(bounds.x, bounds.y),
        cv::Point2f(bounds.x + bounds.width, bounds.y),
        cv::Point2f(bounds.x + bounds.width, bounds.y + bounds.height),
        cv::Point2f(bounds.x, bounds.y + bounds.height)
    };
    
    std::vector<cv::Point2f> objectPoints = {
        cv::Point2f(0, 0),
        cv::Point2f(0.1f, 0),
        cv::Point2f(0.1f, 0.1f),
        cv::Point2f(0, 0.1f)
    };
    
    cv::Mat H = cv::findHomography(imagePoints, objectPoints);
    cv::Point2f center(bounds.x + bounds.width/2, bounds.y + bounds.height/2);
    std::vector<cv::Point2f> transformed;
    cv::perspectiveTransform(std::vector<cv::Point2f>{center}, transformed, H);
    
    return {transformed[0].x, transformed[0].y};
}

bool CameraProcessor::isPainted(const cv::Mat& frame) {
    cv::Mat hsvFrame, mask;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    mask = applyColorMask(hsvFrame);
    return cv::countNonZero(mask) < 500;
}