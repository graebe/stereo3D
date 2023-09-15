#pragma once
#include <opencv2/opencv.hpp>

class CameraInterface
{
private:
    void moveProperties_(CameraInterface &source);

protected:
    std::unique_ptr<cv::Mat> img;
    cv::VideoCapture capt_;
    std::string captDef_{"0"};
    bool isCapturing_{false};

public:
    // Constructor
    CameraInterface();
    // Disallow Copy Construction
    CameraInterface(const CameraInterface &) = delete;
    CameraInterface &operator=(const CameraInterface &) = delete;
    //  Move Construction
    CameraInterface(CameraInterface &&source) noexcept;
    CameraInterface &operator=(CameraInterface &&source) noexcept;
    // Destructor
    ~CameraInterface();
    //  Methods
    virtual void setCaptureDefinition(std::string definition) = 0;
    virtual int startCapture(int warmUpFrames) = 0;
    virtual int capture() = 0;
    virtual cv::Mat getImage() const = 0;
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
    // Destructor
    ~Camera();
    // Properties
    cv::Mat K{cv::Mat()};
    cv::Mat D{cv::Mat()};
    cv::Mat R{cv::Mat()};
    cv::Mat t{cv::Mat()};
    Imager imager;
    float calibrationError;
    // Methods
    void setCaptureDefinition(std::string definition) override;
    int startCapture(int warmUpFrames) override;
    int capture() override;
    cv::Mat getImage() const override;
    void saveImage(std::string filename) override;
    void releaseCapture() override;
};

class MultiCamera
{
private:
    std::vector<Camera> _cams;

public:
    // Constructors
    MultiCamera();
    MultiCamera(std::vector<Camera> cams);
    // Methods
    void addCamera(Camera &&cam);
    Camera* getCamera(int i_camera);
    int size();
    void startCapture(int warmUpFrames);
    void capture();
    cv::Mat getImage(int i_camera) const;
    void releaseCapture();
    void saveImages(std::string filename);
};
