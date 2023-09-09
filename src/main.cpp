#include "camera.hpp"
#include "calibration.hpp"
#include <opencv2/opencv.hpp>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

std::atomic<bool> run(true);
cv::Mat imgShared;
std::mutex imgMutex;
std::condition_variable imgAvail;

void display()
{
    while (run)
    {
        cv::Mat imgShow;
        {
            std::unique_lock<std::mutex> lock(imgMutex);
            imgAvail.wait(lock, []
                          { return !imgShared.empty(); });
            if (imgShared.empty())
            {
                continue;
            }
            imgShow = imgShared.clone();
            imgShared.release();
        }
        cv::imshow("Camera Stream", imgShow);
    }
}

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
    // cam1.setCaptureDefinition(std::string("nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
    cam1.setCaptureDefinition(std::string("0"));

    // Camera 2
    std::cout << "\nInitializing Caputre Camera 2" << std::endl;
    Camera cam2 = Camera();
    cam2.setCaptureDefinition(std::string("0"));
    // cam2.setCaptureDefinition(std::string("nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));

    // Multi Cameras
    std::cout << "Initializing Multi Camera" << std::endl;
    MultiCamera cams = MultiCamera();
    cams.addCamera(std::move(cam1));
    cams.addCamera(std::move(cam2));

    // Capture Image When Space Is Pressed
    std::cout << "Starting Multi Camera Capture ..." << std::endl;
    cams.startCapture(5);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(-1);

    int ch;
    std::string filename = "calibration_img.jpg";
    size_t pos = filename.find(".");

    std::thread dispThread(display);

    int i = 0;
    while (true)
    {
        i++;
        ch = getch();
        if (ch == ' ')
        {
            std::cout << "Capturing Frame." << std::endl;
            cams.capture();
            {
                std::lock_guard<std::mutex> lock(imgMutex);
                imgShared = std::move(cams.getImage(0));
                imgAvail.notify_one();
            }
            cams.saveImages("calibration_img_" + std::to_string(i) + "_.jpg");
        }
        if (ch == 'q')
        {
            std::cout << "Quitting" << std::endl;
            break;
        }
    }
    endwin();
    cams.releaseCapture();

    dispThread.join();

    return 0;
}
