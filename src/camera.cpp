#include "camera.hpp"

void CameraInterface::moveProperties_(CameraInterface &source)
{
    this->capt_ = std::move(source.capt_);
    this->img = std::move(source.img);
    this->isCapturing_ = std::move(source.isCapturing_);
}

CameraInterface::CameraInterface(CameraInterface &&source) noexcept
{
    moveProperties_(source);
}

CameraInterface &CameraInterface::operator=(CameraInterface &&source) noexcept
{
    if (this == &source)
    {
        return *this;
    }
    moveProperties_(source);
    return *this;
};

// Constructors
Camera::Camera() : CameraInterface(){};
Camera::Camera(cv::Mat K_, cv::Mat D_) : K(K_), D(D_){};

// Move Constructors
void Camera::moveProperties_(Camera &source)
{
    this->K = std::move(source.K);
    this->D = std::move(source.D);
    this->R = std::move(source.R);
    this->t = std::move(source.t);
    this->imager = std::move(source.imager);
    this->calibrationError = std::move(source.calibrationError);
};

Camera::Camera(Camera &&source) noexcept : CameraInterface(std::move(source))
{
    moveProperties_(source);
}

Camera &Camera::operator=(Camera &&source) noexcept
{
    // Check if self
    if (this == &source)
    {
        return *this;
    }
    // Camera Interface
    CameraInterface::operator=(std::move(source));
    moveProperties_(source);
    // Return
    return *this;
};

// Destructor
Camera::~Camera() { Camera::releaseCapture(); };

// Methods
int Camera::startCapture(int warmUpFrames)
{
    std::cout << "Capturing photo with Camera..." << std::endl;
    capt_ = cv::VideoCapture(0); //("nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", cv::CAP_GSTREAMER);
    if (!capt_.isOpened())
    {
        printf("Cam is not opened.\n");
        isCapturing_ = false;
        return -1;
    }
    else
    {
        isCapturing_ = true;
    }
    for (int i = 0; i < warmUpFrames; ++i)
    {
        capt_ >> img;
    }
    return 0;
};

int Camera::capture()
{
    capt_ >> img;
    if (img.empty())
    {
        std::cerr << "Error: Captured frame is empty." << std::endl;
        return -1;
    }
    return 0;
};

cv::Mat Camera::getImage() { return img; };

void Camera::saveImage(std::string filename)
{
    if (!img.empty())
    {
        cv::imwrite(filename, img);
    }
    else
    {
        std::cout << "Could not write empty image." << std::endl;
    }
};

void Camera::releaseCapture()
{
    if (isCapturing_)
    {
        capt_.release();
    }
}
