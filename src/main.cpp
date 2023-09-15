#include "camera.hpp"
#include "calibration.hpp"
#include <cstring>
#include <chrono>

int main(int argc, char** argv)
{
    // Read Arguments 
    std::string captureDef0 = "0";
    std::string captureDef1 = "0";
    for (int i = 1; i<argc; i++) {
	if(strcmp(argv[i],"--cam=0") == 0) {
            captureDef0 = "0";	
            captureDef1 = "0";	
	}
	if(strcmp(argv[i],"--cam=jetson") == 0) {
            captureDef0 = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	    captureDef1 = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=640, height=480, format=(string)NV12, framerate=(fraction)20/1 ! nvvidconv flip-method=0 ! video/x-raw, width=640, height=480, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	}
    }

    // Create Cameras
    Camera cam1 = Camera();
    Camera cam2 = Camera();
    cam1.setCaptureDefinition(captureDef0);
    cam2.setCaptureDefinition(captureDef1);
    
    MultiCamera cams = MultiCamera();
    cams.addCamera(std::move(cam1));
    cams.addCamera(std::move(cam2));

    // Start Capture
    cams.startCapture(1);

    cv::Mat imgShared;
    int i = 0;
    bool run = true;  // Using a regular boolean instead of atomic<bool>

    while (run)
    {
        i++;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::cout << "Capturing Frame." << std::endl;
        cams.capture();

        imgShared.release();
        imgShared = cams.getImage(0);

        cams.saveImages("calibration_img_" + std::to_string(i) + "_.jpg");

        // Listen to key
        int key = cv::waitKey(30);
        if (key == 'q')
        {
            std::cout << "Quitting" << std::endl;
            run = false;
            continue;
        }

        // Plot Image if Available
        if (imgShared.empty())
        {
            std::cout << "Image is empty" << std::endl;
        }
        else
        {
            std::cout << "Plotting Image" << std::endl;
            cv::imshow("Camera Stream", imgShared);
        }
    }

    return 0;
}

