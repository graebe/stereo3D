#include "camera.hpp"

// Constructors
Camera::Camera(){};
Camera::Camera(cv::Mat K_, cv::Mat D_) : K(K_), D(D_){};

int Camera::initCapture(int warmUpFrames)
{
    std::cout << "Capturing photo with Camera..." << std::endl;
    capt = cv::VideoCapture(0); //("nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", cv::CAP_GSTREAMER);
    if (!capt.isOpened())
    {
        printf("Cam is not opened.\n");
        return -1;
    }
    for (int i = 0; i < warmUpFrames; ++i)
    {
        capt >> img;
    }
    return 0;
};

int Camera::capture()
{
    capt >> img;
    if (img.empty())
    {
        std::cerr << "Error: Captured frame is empty." << std::endl;
        return -1;
    }
    return 0;
};

cv::Mat Camera::getImage() { return img; };

void Camera::saveImage(std::string filename) { cv::imwrite(filename, img); };

void Camera::releaseCapture()
{
    capt.release();
}