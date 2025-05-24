#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class CameraProcessor {
public:
    CameraProcessor();
    virtual ~CameraProcessor();
    
    void setColorRange(const cv::Scalar& lower, const cv::Scalar& upper);
    std::vector<double> processFrameWithHomography(const cv::Mat& frame);
    bool isPainted(const cv::Mat& frame);
    cv::Mat getColorMask(const cv::Mat& frame);  // Публичный метод для доступа к маске

protected:
    cv::Mat applyColorMask(const cv::Mat& hsvFrame);  // Реализуем в .cpp файле

private:
    cv::Scalar lowerColor, upperColor;
    std::vector<cv::Point> findContours(const cv::Mat& mask);
};