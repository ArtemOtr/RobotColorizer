#ifndef SERVER_H
#define SERVER_H

#include <opencv2/opencv.hpp>
#include <memory>
#include <vector>
#include <string>
#include "camera_processor.h"

class Server {
public:
    explicit Server(const std::string& camera_url);
    ~Server();

    cv::Mat process_frame();
    std::vector<double> get_graffiti_location_with_homography(bool show_windows = false);
    bool is_graffiti_painted();
    void set_min_pixels(int min_pixels);

private:
    cv::VideoCapture camera_;
    std::unique_ptr<CameraProcessor> processor_;
    int min_pixels_ = 1000;
};

#endif // SERVER_H