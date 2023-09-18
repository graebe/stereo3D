#include "camera.hpp"
#include "calibration.hpp"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <cstring>

std::atomic<bool> run(true);
std::mutex imgMutex;
std::condition_variable imgUpdated;

void capture(const std::reference_wrapper<MultiCamera> camsSharedWrapper,
             std::reference_wrapper<cv::Mat> imgSharedWrapper0,
             std::reference_wrapper<cv::Mat> imgSharedWrapper1)
{

    MultiCamera &cams = camsSharedWrapper.get();
    cv::Mat &imgShared0 = imgSharedWrapper0.get();
    cv::Mat &imgShared1 = imgSharedWrapper1.get();

    cams.startCapture(1);

    while (run)
    {
        cams.capture();

        {
            std::lock_guard<std::mutex> lock(imgMutex);
            imgShared0 = cams.getImage(0);
            imgShared1 = cams.getImage(1);
        }

        imgUpdated.notify_one();
    }
}

int main(int argc, char *argv[])
{

    std::string captureDef0 = "0";
    std::string captureDef1 = "0";
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--cam=0") == 0)
        {
            captureDef0 = "0";
            captureDef1 = "0";
        }
        else if (strcmp(argv[i], "--cam=jetson") == 0)
        {
            captureDef0 = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
            captureDef1 = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
        }
    }

    Camera cam1;
    Camera cam2;
    cam1.setCaptureDefinition(captureDef0);
    cam2.setCaptureDefinition(captureDef1);

    MultiCamera cams;
    cams.addCamera(std::move(cam1));
    cams.addCamera(std::move(cam2));

    CalibrationBoard B(9, 6, 25.0);
    Calibrator C(B);

    cv::Mat imgShared0;
    cv::Mat imgShared1;
    std::thread captureThread(capture, std::ref(cams), std::ref(imgShared0), std::ref(imgShared1));

    int i = 0;
    clock_t lastCapture{clock()};

    while (run)
    {
        {
            std::unique_lock<std::mutex> lock(imgMutex);
            imgUpdated.wait(lock); // Wait for a new frame

            if (!imgShared0.empty())
                cv::imshow("Camera Stream 0", imgShared0);

            if (!imgShared1.empty())
                cv::imshow("Camera Stream 1", imgShared1);
        }

        int key = cv::waitKey(5);

        if (key == ' ' && static_cast<double>(clock() - lastCapture) / CLOCKS_PER_SEC > 1.0)
        {
            // Save the frames, for example:i
            cv::imwrite("img_left_" + std::to_string(i) + ".jpg", imgShared0);
            cv::imwrite("img_right_" + std::to_string(i) + ".jpg", imgShared1);
            i++;
            lastCapture = clock();

            // Calibrate
            C.addCalibrationImages(imgShared0);
            std::cout << "Number of Calibration Frames: " << std::to_string(C.size()) << std::endl;
            std::cout << "Calibrating ..." << std::endl;
            C.checkerboardCalibration(cams.getCamera(0));

            // Output the results
            C.printSummary();
            std::cout << "Camera Matrix: \n"
                      << cam1.K << std::endl;
            std::cout << "Distortion Coefficients: \n"
                      << cam1.D << std::endl;
        }

        if (key == 'q')
        {
            std::cout << "Quitting" << std::endl;
            run = false;
        }
    }

    captureThread.join();

    return 0;
}
