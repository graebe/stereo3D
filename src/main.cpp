#include "camera.hpp"
#include "calibration.hpp"

int main()
{
    // Create Camera
    Camera cam1 = Camera();

    // Create Calibrator
    CalibrationBoard B = CalibrationBoard(9, 6, 25.0);
    Calibrator C = Calibrator(B);

    // List of Calibration Files
    std::vector<std::string> calibrationImages = {"img_test_01.jpg", "img_test_02.jpg", "img_test_03.jpg"};

    // Calibrate
    C.checkerboardCalibration(calibrationImages, cam1);

    // Output the results
    std::cout << "Camera Matrix: \n"
              << cam1.getK() << std::endl;
    std::cout << "Distortion Coefficients: \n"
              << cam1.getD() << std::endl;
    std::cout << "success." << std::endl;

    return 0;
}
