#include "camera.hpp"
#include "calibration.hpp"
#include <cstring>
#include <chrono>
#include <ctime>

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

    // Create Calibrator
    CalibrationBoard B = CalibrationBoard(9, 6, 25.0);
    Calibrator C = Calibrator(B);

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

    // Preparation
    int i = 0;
    clock_t lastCapture{clock()};
    bool run = true;

    // Run Capture Loop
    while (run)
    {
        // Capture Frame
        cams.capture();


        // Save Frame if Space pressed, Quit with q 
        int key = cv::waitKey(5);
        if (key == 'q')
        {
            std::cout << "Quitting" << std::endl;
            run = false;
            continue;
        }
	if (key == ' ' && static_cast<double>(clock() - lastCapture) / CLOCKS_PER_SEC > 1.0) {

	    // Capture Frame
	    std::cout << "Caputring Frame" << std::endl;
            cams.saveImages("img_" + std::to_string(i) + "_.jpg");
	    i++;
	    lastCapture = clock();

	    // Calibrate
	    C.addCalibrationImages(cams.getImage(0));
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

        // Plot Image if Available
        if (cams.getImage(0).empty() || cams.getImage(1).empty())
        {
            std::cout << "Image is empty" << std::endl;
        }
        else
        {
            cv::Mat img0 = cams.getImage(0);
	    cv::Mat img1 = cams.getImage(1); 
            cv::imshow("Camera Stream 0", img0);
            cv::imshow("Camera Stream 1", img1);
        }
    }

    return 0;
}

