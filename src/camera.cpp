#include "camera.hpp"

// *********************************** CAMERA ***********************************
// Camera Interface Constructors
CameraInterface::CameraInterface() { img = std::make_unique<cv::Mat>(); };
CameraInterface::~CameraInterface(){};

// Camera Interface Move Constructors
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

// Camera Constructors
Camera::Camera() : CameraInterface(){};
Camera::Camera(cv::Mat K_, cv::Mat D_) : K(K_), D(D_){};

// Camera Move Constructors
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
    cv::Mat imgTmp;
    capt_ = cv::VideoCapture(0); //("nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", cv::CAP_GSTREAMER);
    if (!capt_.isOpened())
    {
        std::cerr << "Error: Failed to open the camera. Ensure the device is connected and permissions are set." << std::endl;
        isCapturing_ = false;
        return -1;
    }
    isCapturing_ = true;
    for (int i = 0; i < warmUpFrames; ++i)
    {
        capt_ >> imgTmp;
        if (imgTmp.empty())
        {
            std::cerr << "Warning: Failed to capture image during warmup." << std::endl;
            isCapturing_ = false;
            return -2;
        }
    }
    img = std::make_unique<cv::Mat>(std::move(imgTmp));
    return 0;
};

int Camera::capture()
{
    cv::Mat imgTmp;
    capt_ >> imgTmp;
    if (imgTmp.empty())
    {
        std::cerr << "Error: Failed to capture image." << std::endl;
        isCapturing_ = false;
        return -1;
    }
    img = std::make_unique<cv::Mat>(std::move(imgTmp));
    return 0;
};

cv::Mat Camera::getImage()
{
    if (img)
    {
        return *img;
    }
    else
    {
        return cv::Mat();
    }
};

void Camera::saveImage(std::string filename)
{
    if (!img->empty())
    {
        cv::imwrite(filename, *img);
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

// ***************************** MULTI CAMERA ***********************************

MultiCamera::MultiCamera(){};

void MultiCamera::addCamera(Camera &&cam)
{
    _cams.emplace_back(std::move(cam));
};

void MultiCamera::startCapture(){};

void MultiCamera::capture(){};
