#include "camera.hpp"
#include "calibration.hpp"
#include <opencv2/opencv.hpp>

int main()
{
    // Create Camera
    Camera cam1 = Camera();
    cam1.imager.psize = cv::Size(640, 480);

    // Create Calibrator
    CalibrationBoard B = CalibrationBoard(9, 6, 25.0);
    Calibrator C = Calibrator(B);

    // List of Calibration Files
    std::vector<std::string> calibrationImages = {"img_test_01.jpg", "img_test_02.jpg", "img_test_03.jpg"};

    // Calibrate
    C.addCalibrationImages(calibrationImages);
    C.checkerboardCalibration(cam1);

    // Output the results
    C.printSummary();
    std::cout << "Camera Matrix: \n"
              << cam1.K << std::endl;
    std::cout << "Distortion Coefficients: \n"
              << cam1.D << std::endl;
    std::cout << "success." << std::endl;

    // Camera 1
    std::cout << "\nInitializing Caputre Camera 1" << std::endl;
    cam1.setCaptureDefinition(std::string("nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
    cam1.startCapture(5);
    cam1.capture();
    cam1.releaseCapture();
    cam1.saveImage("cap1.jpg");
    cv::Mat img1 = cam1.getImage();

    // Camera 2
    std::cout << "\nInitializing Caputre Camera 2" << std::endl;
    Camera cam2 = Camera();
    cam2.setCaptureDefinition(std::string("nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
    cam2.startCapture(5);
    cam2.capture();
    cam2.releaseCapture();
    cam2.saveImage("cap2.jpg");
    cv::Mat img2 = cam2.getImage();

    // Multi Cameras
    MultiCamera cams = MultiCamera();
    cams.addCamera(std::move(cam1));
    cams.addCamera(std::move(cam2));
    std::cout << "Starting Multi Camera Capture ..." << std::endl;
    cams.startCapture(5);
    std::cout << "Capturing Multi Camera ..." << std::endl;
    cams.capture();
    cams.releaseCapture();
    std::cout << "Saving Images ..." << std::endl;
    cams.saveImages("captmulti.jpg");

    return 0;
}
