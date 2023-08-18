#include "camera.hpp"

// Constructors
Camera::Camera(){};
Camera::Camera(cv::Mat K_, cv::Mat D_) : K(K_), D(D_){};
