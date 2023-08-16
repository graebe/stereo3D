#include "camera.hpp"

// Constructors
Camera::Camera(){};
Camera::Camera(cv::Mat K, cv::Mat D) : K_(K), D_(D){};

// Setter
void Camera::setK(cv::Mat K) { K_ = K; }
void Camera::setD(cv::Mat D) { D_ = D; }
void Camera::setCalibrationError(float error) { calibrationError_ = error; }

// Getter
cv::Mat Camera::getK() { return K_; }
cv::Mat Camera::getD() { return D_; }
float Camera::getCalibrationError() { return calibrationError_; }
