#include <gtest/gtest.h>
#include <filesystem>
#include "../src/camera.hpp"

TEST(CameraTest, DefaultConstructor)
{
    Camera cam;
    ASSERT_TRUE(cam.getImage().empty());
}

TEST(CameraTest, ParametrizedConstructor)
{
    cv::Mat K = (cv::Mat_<double>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
    cv::Mat D = cv::Mat::zeros(1, 5, CV_64F);
    Camera cam(K, D);

    // Test if matrices are set correctly.
    ASSERT_EQ(cv::countNonZero(cam.K != K), 0);
    ASSERT_EQ(cv::countNonZero(cam.D != D), 0);
}

TEST(CameraTest, MoveAssignment)
{
    // Test data
    cv::Mat K = (cv::Mat_<double>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
    cv::Mat D = cv::Mat::zeros(1, 5, CV_64F);
    Imager img = {0.01f, 0.02f, cv::Size(1920, 1080)};

    Camera cam1(K, D);
    cam1.R = cv::Mat::eye(3, 3, CV_64F);
    cam1.t = cv::Mat::ones(3, 1, CV_64F);
    cam1.imager = img;
    cam1.calibrationError = 0.001f;

    cam1.startCapture(3);

    Camera cam2;

    // Move assign cam1 to cam2
    cam2 = std::move(cam1);

    // Test if properties are transferred correctly.
    ASSERT_TRUE(cam1.getImage().empty());
    ASSERT_FALSE(cam2.getImage().empty());

    // Check matrices
    ASSERT_EQ(cv::countNonZero(cam2.K != K), 0);
    ASSERT_EQ(cv::countNonZero(cam2.D != D), 0);
    ASSERT_EQ(cv::countNonZero(cam2.R != cv::Mat::eye(3, 3, CV_64F)), 0);
    ASSERT_EQ(cv::countNonZero(cam2.t != cv::Mat::ones(3, 1, CV_64F)), 0);

    // Check other properties
    ASSERT_EQ(cam2.imager.pw, 0.01f);
    ASSERT_EQ(cam2.imager.ph, 0.02f);
    ASSERT_EQ(cam2.imager.psize, cv::Size(1920, 1080));
    ASSERT_FLOAT_EQ(cam2.calibrationError, 0.001f);
}

TEST(CameraTet, MoveAssignment)
{
    Camera cam1;
    cam1.startCapture(3);
    Camera cam2;
    cam2 = std::move(cam1);

    // Test if properties are transferred correctly.
    ASSERT_TRUE(cam1.getImage().empty());
    ASSERT_FALSE(cam2.getImage().empty());
}

TEST(CameraTest, CheckCaptureImageNotEmpty)
{
    Camera cam;
    cam.startCapture(1);
    cam.capture();

    // Test if the captured image is not empty.
    ASSERT_FALSE(cam.getImage().empty());
}

TEST(CameraTest, SaveImage)
{
    Camera cam;
    cam.startCapture(1);
    cam.capture();
    std::string filename = "test_image.jpg";
    cam.saveImage(filename);

    // Test if the image was saved correctly by checking if the file exists.
    ASSERT_TRUE(std::filesystem::exists(filename));

    // Cleanup: remove the saved test image.
    std::filesystem::remove(filename);
}