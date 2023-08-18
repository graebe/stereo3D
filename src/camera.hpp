#pragma once
#include <opencv2/opencv.hpp>

class Camera
{
private:
public:
    Camera();
    Camera(cv::Mat K, cv::Mat D);
    cv::Mat K;
    cv::Mat D;
    cv::Mat R;
    cv::Mat t;
    float calibrationError;
};

class StereoCamera
{
private:
    Camera C1;
    Camera C2;

public:
    StereoCamera(){};
};