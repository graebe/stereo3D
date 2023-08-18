#pragma once
#include <opencv2/opencv.hpp>

struct Imager
{
    float pw;
    float ph;
    cv::Size psize;
};

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
    Imager imager;
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