#pragma once
#include <opencv2/opencv.hpp>

class CameraInterface
{
private:
    void moveProperties_(CameraInterface &source);

protected:
    cv::Mat img;
    cv::VideoCapture capt_;
    bool isCapturing_{false};

public:
    CameraInterface(){};
    // Disallow Copy Construction
    CameraInterface(const CameraInterface &) = delete;
    CameraInterface &operator=(const CameraInterface &) = delete;
    //  Move Construction
    CameraInterface(CameraInterface &&source) noexcept;
    CameraInterface &operator=(CameraInterface &&source) noexcept;
    //  Methods
    virtual int startCapture(int warmUpFrames) = 0;
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
    void moveProperties_(Camera &source);

public:
    // Constructors
    Camera();
    Camera(cv::Mat K, cv::Mat D);
    // Copy Constructor (deleted in interface)
    Camera(Camera &&source) noexcept;
    // Copy Assignment Operator (deleted in interface)
    Camera &operator=(Camera &&source) noexcept;
    ~Camera();
    // Properties
    cv::Mat K;
    cv::Mat D;
    cv::Mat R;
    cv::Mat t;
    Imager imager;
    float calibrationError;
    // Methods
    int startCapture(int warmUpFrames) override;
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
    void startCapture();
    void capture();
};