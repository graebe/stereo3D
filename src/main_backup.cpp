#include "camera.hpp"
#include "calibration.hpp"
//#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

std::atomic<bool> run(true);
std::mutex imgMutex;
std::condition_variable imgAvail;

void capture(std::reference_wrapper<cv::Mat> imgSharedWrapper)
{
    // Extract imgShared
    cv::Mat& imgShared = imgSharedWrapper.get();
    // Create Cameras
    Camera cam1 = Camera();
    Camera cam2 = Camera();
    MultiCamera cams = MultiCamera();
    // cam1.setCaptureDefinition(std::string("0"));
    // cam2.setCaptureDefinition(std::string("0"));
    cam1.setCaptureDefinition(std::string("nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
    cam2.setCaptureDefinition(std::string("nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
    cams.addCamera(std::move(cam1));
    cams.addCamera(std::move(cam2));

    // Start Capture
    cams.startCapture(5);

    // Main Loop
    int ch;
    int i = 0;
    while (run)
    {
        i++;
	ch = ' ';
        if (ch == ' ')
        {
            std::cout << "Capturing Frame." << std::endl;
            cams.capture();
            {
                std::lock_guard<std::mutex> lock(imgMutex);
		imgShared.release();
                imgShared = cams.getImage(0);
                imgAvail.notify_one();
            }
            cams.saveImages("calibration_img_" + std::to_string(i) + "_.jpg");
        }
        if (ch == 'q')
        {
            std::cout << "Quitting" << std::endl;
            run = false;
            imgAvail.notify_all();
        }
    }
}

int main()
{

    // Create Calibrator
    // CalibrationBoard B = CalibrationBoard(9, 6, 25.0);
    // Calibrator C = Calibrator(B);

    // List of Calibration Files
    // std::vector<std::string> calibrationImages = {"img_test_01.jpg", "img_test_02.jpg", "img_test_03.jpg"};

    // Calibrate
    // C.addCalibrationImages(calibrationImages);
    // C.checkerboardCalibration(cam1);

    // Output the results
    // C.printSummary();
    // std::cout << "Camera Matrix: \n"
    //           << cam1.K << std::endl;
    // std::cout << "Distortion Coefficients: \n"
    //           << cam1.D << std::endl;
    // std::cout << "success." << std::endl;
    
    
    std::cout << "Starting to Capture...";
    cv::Mat imgShared;
    std::thread captureThread(capture, std::ref(imgShared));

    while (run)
    {
        {
	    // Wait for Lock
            std::unique_lock<std::mutex> lock(imgMutex);
            imgAvail.wait(lock, [&imgShared]
                          { return !imgShared.empty(); });

	    // Plot Image if Available
            if (imgShared.empty())
            {
	        std::cout << "Image is empty" << std::endl;
                continue;
            }
	    else
	    {
	        std::cout << "Plotting Image" << std::endl;
	        cv::imshow("Camera Stream", imgShared);
	    }
	}
    }

    // Clean Up
    captureThread.join();
    return 0;
}
