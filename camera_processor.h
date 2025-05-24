#ifndef CAMERA_PROCESSOR_H
#define CAMERA_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

class CameraProcessor {
public:
    CameraProcessor();
    ~CameraProcessor();

    cv::Mat getColorMask(const cv::Mat& frame);
    std::vector<double> processFrameWithMask(const cv::Mat& frame); // Измененное имя метода
    cv::Mat applyColorMasks(const cv::Mat& hsvFrame);

private:
    std::vector<std::pair<cv::Scalar, cv::Scalar>> colorRanges;
    const int min_contour_area = 500; // Минимальная площадь контура
};

#endif // CAMERA_PROCESSOR_H