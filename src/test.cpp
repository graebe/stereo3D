#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <atomic>

#include "camera.hpp"

std::mutex imgMutex;
cv::Mat imgShared;
std::atomic<bool> run(true);

void displayImage() {
    while (run) {
        cv::Mat localCopy;
        {
            std::lock_guard<std::mutex> lock(imgMutex);
            localCopy = imgShared.clone();
        }
        
        if (!localCopy.empty()) {
            cv::imshow("Dummy Image", localCopy);
            cv::waitKey(30);  // Wait for a short time before the next frame
        }
    }
}

int main() {
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
    cams.startCapture(5);

    // Create a thread to display the image
    std::thread displayThread(displayImage);

    // Generate and update the dummy image in the main thread
    while (run) {
	cams.capture();
	cv::Mat camImage = cams.getImage(0);
        cv::Mat dummyImage = cv::Mat::zeros(480, 640, CV_8UC3);  // Black image
        cv::randu(dummyImage, cv::Scalar(0,0,0), cv::Scalar(255,255,255));  // Random color noise
        
        {
            std::lock_guard<std::mutex> lock(imgMutex);
            imgShared = camImage.clone();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Update every second

        // Let's run this for 10 iterations for demonstration purposes.
        // You can use any other termination condition as needed.
        static int iterations = 0;
        iterations++;
        if (iterations >= 10) {
            run = false;
        }
    }

    // Ensure the display thread terminates properly
    displayThread.join();

    return 0;
}

