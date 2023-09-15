#include "camera.hpp"

// *********************************** CAMERA ***********************************
// Camera Interface Constructors
CameraInterface::CameraInterface() { img = std::make_unique<cv::Mat>(); };
CameraInterface::~CameraInterface(){};

// Camera Interface Move Constructors
void CameraInterface::moveProperties_(CameraInterface &source)
{
    this->capt_ = std::move(source.capt_);
    this->captDef_ = std::move(source.captDef_);
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
void Camera::setCaptureDefinition(std::string definition)
{
    captDef_ = definition;
}

int Camera::startCapture(int warmUpFrames)
{
    cv::Mat imgTmp;
    if (captDef_ == "0")
    {
        capt_ = cv::VideoCapture(0);
    }
    else
    {
        capt_ = cv::VideoCapture(captDef_);
    }
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

cv::Mat Camera::getImage () const
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

Camera* MultiCamera::getCamera(int i_camera) {
    return &_cams[i_camera];
}

int MultiCamera::size()
{
    return _cams.size();
}

void MultiCamera::startCapture(int warmUpFrames)
{
    for (Camera &_cam : _cams)
    {
        _cam.startCapture(warmUpFrames);
    }
};

void MultiCamera::capture()
{
    for (Camera &_cam : _cams)
    {
        _cam.capture();
    }
};

cv::Mat MultiCamera::getImage(int i_camera) const
{
    return _cams[i_camera].getImage();
};

void MultiCamera::releaseCapture()
{
    for (Camera &_cam : _cams)
    {
        _cam.releaseCapture();
    }
};

void MultiCamera::saveImages(std::string filename)
{
    size_t idxDot = filename.find_last_of('.');
    for (int i = 0; i < size(); i++)
    {
        std::string currFilename;
        if (!(idxDot == std::string::npos))
        {
            std::string base = filename.substr(0, idxDot);
            std::string extension = filename.substr(idxDot);
            currFilename = base + std::to_string(i) + extension;
        }
        else
        {
            currFilename = filename + std::to_string(i);
        }
        _cams[i].saveImage(currFilename);
    }
}
