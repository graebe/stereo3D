#pragma once
#include <opencv2/opencv.hpp>

class Camera
{
private:
    cv::Mat K_;
    cv::Mat D_;
    float calibrationError_;

public:
    Camera();
    Camera(cv::Mat K, cv::Mat D);
    void setK(cv::Mat K);
    void setD(cv::Mat D);
    void setCalibrationError(float error);
    cv::Mat getK();
    cv::Mat getD();
    float getCalibrationError();
};

class StereoCamera
{
private:
    Camera C1;
    Camera C2;

public:
    StereoCamera(){};
};