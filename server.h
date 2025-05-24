#ifndef SERVER_H
#define SERVER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class Server {
public:
    Server(const std::string& camera_url = "0",
           const cv::Scalar& lower_color = cv::Scalar(0, 120, 70),
           const cv::Scalar& upper_color = cv::Scalar(10, 255, 255));
    
    ~Server();

    // Основные методы
    std::vector<double> get_graffiti_location_with_homography(bool show_windows = true);
    bool is_graffiti_painted();
    
    // Настройки детекции
    void set_color_range(const cv::Scalar& lower, const cv::Scalar& upper);
    void set_min_pixels(int min_pixels);

private:
    cv::VideoCapture camera_;
    cv::Scalar lower_color_, upper_color_;
    int min_pixels_ = 500;
    
    cv::Mat process_frame();
    cv::Mat apply_color_mask(const cv::Mat& hsv_frame);
};

#endif // SERVER_H