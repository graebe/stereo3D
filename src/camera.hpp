#pragma once
#include <opencv2/opencv.hpp>

class CameraInterface
{
protected:
    cv::Mat img;
    cv::VideoCapture capt;

public:
    // Methods
    virtual int initCapture(int warmUpFrames) = 0;
    virtual int capture() = 0;
    virtual cv::Mat getImage() = 0;
    virtual void saveImage(std::string filename) = 0;
    virtual void releaseCapture() = 0;
};

struct Imager
{
    float pw;
    float ph;
    cv::Size psize;
};

class Camera : public CameraInterface
{
private:
public:
    // Constructors
    Camera();
    Camera(cv::Mat K, cv::Mat D);
    // Properties
    cv::Mat K;
    cv::Mat D;
    cv::Mat R;
    cv::Mat t;
    Imager imager;
    float calibrationError;
    // Methods
    int initCapture(int warmUpFrames) override;
    int capture() override;
    cv::Mat getImage() override;
    void saveImage(std::string filename) override;
    void releaseCapture() override;
};

class StereoCamera
{
private:
    Camera C1;
    Camera C2;

public:
    StereoCamera(){};
    void initCapture();
    void capture();
};