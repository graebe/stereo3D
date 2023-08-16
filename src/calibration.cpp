#include "calibration.hpp"

// Constructors
Calibrator::Calibrator(){};
Calibrator::Calibrator(CalibrationBoard board) : board_(board){};

// Calibration
bool Calibrator::checkerboardCalibration(const std::vector<std::string> files, Camera &C)
{
    int n_images = 3;
    std::vector<cv::Point3f> obj;
    for (int i = 0; i < board_.boardSize.height; i++)
        for (int j = 0; j < board_.boardSize.width; j++)
            obj.push_back(cv::Point3f(j * board_.sqSize, i * board_.sqSize, 0.0f));

    for (int img_idx = 0; img_idx < files.size(); img_idx++)
    {
        // Load the image in color mode
        std::cout << "reading image " << files[img_idx] << std::endl;
        cv::Mat image = cv::imread(files[img_idx], cv::IMREAD_COLOR);
        if (image.empty())
        {
            std::cerr << "Failed to load image: " << files[img_idx] << std::endl;
            continue;
        }

        std::vector<cv::Point2f> corners;
        bool pattern_found = cv::findChessboardCorners(image, board_.boardSize, corners);

        if (pattern_found)
        {
            object_points.push_back(obj);

            // Convert the image to grayscale for cornerSubPix
            cv::Mat gray_image;
            cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
            cv::cornerSubPix(gray_image, corners, cv::Size(11, 11), cv::Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
            image_points.push_back(corners);

            // Draw the identified points in color on the original image
            cv::drawChessboardCorners(image, board_.boardSize, corners, pattern_found);
            std::string output_filename = "output_image_0" + std::to_string(img_idx) + ".jpg";
            cv::imwrite(output_filename, image);
        }
        else
        {
            std::cerr << "Checkerboard not found in the image: " << files[img_idx] << std::endl;
        }
    }
    if (object_points.size() < 2 || image_points.size() < 2)
    {
        std::cerr << "Insufficient valid images for calibration." << std::endl;
        return false;
    }

    cv::Mat K, D;
    std::vector<cv::Mat> rvecs, tvecs;

    // Calibrate the camera
    double error = cv::calibrateCamera(object_points, image_points, cv::Size(640, 480), K, D, rvecs, tvecs);

    // Set Camera Intrinsics
    C.setK(K);
    C.setD(D);
    C.setCalibrationError(error);

    // Return
    return true;
};