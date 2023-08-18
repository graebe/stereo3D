#include "calibration.hpp"
#include <stdio.h>

// Constructors
Calibrator::Calibrator(){};
Calibrator::Calibrator(CalibrationBoard board) : board_(board){};

// Helper Functions
void Calibrator::fillCheckerBoardObjectPoints(CalibrationPoints &calpoints)
{
    calpoints.image_points.clear();
    calpoints.object_points.clear();
    for (int i = 0; i < board_.boardSize.height; i++)
        for (int j = 0; j < board_.boardSize.width; j++)
            calpoints.object_points.push_back(cv::Point3f(j * board_.sqSize, i * board_.sqSize, 0.0f));
}

cv::Mat loadColorImage(std::string path)
{
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty())
    {
        std::cerr << "Failed to load image: " << path << std::endl;
    }
    return image;
}

void drawChessboardCorners(cv::Mat image, CalibrationBoard board, std::vector<cv::Point2f> corners, bool pattern_found, std::string filename)
{
    cv::drawChessboardCorners(image, board.boardSize, corners, pattern_found);
    cv::imwrite(filename, image);
}

bool Calibrator::findChessboardCorners(CalibrationPoints &calpoints, cv::Mat &image)
{
    bool pattern_found = cv::findChessboardCorners(image, board_.boardSize, calpoints.image_points);
    if (pattern_found)
    {
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        cv::cornerSubPix(gray_image, calpoints.image_points, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
    }
    return pattern_found;
}

bool Calibrator::addCalibrationImage(std::string file)
{
    // Load Image
    cv::Mat image = loadColorImage(file);
    if (image.empty())
    {
        return false;
    }

    // Find Corners
    CalibrationPoints calpoints;
    fillCheckerBoardObjectPoints(calpoints);
    bool pattern_found = findChessboardCorners(calpoints, image);
    if (pattern_found)
    {
        calsets.object_point_set.push_back(calpoints.object_points);
        calsets.image_point_set.push_back(calpoints.image_points);
        return true;
    }
    else
    {
        std::cerr << "Checkerboard not found in the image: " << file << std::endl;
    }

    // Return
    return false;
}

// Calibration
bool Calibrator::checkerboardCalibration(const std::vector<std::string> files, Camera &C)
{
    // temporary object and corner points
    CalibrationPoints calpoints;
    fillCheckerBoardObjectPoints(calpoints);

    // Load Image, Find Corners
    for (int img_idx = 0; img_idx < files.size(); img_idx++)
    {
        bool pattern_found = addCalibrationImage(files[img_idx]);
        if (~pattern_found)
        {
            continue;
        }
    }

    // Calibrate the camera
    cv::Mat K, D;
    std::vector<cv::Mat> rvecs, tvecs;
    C.calibrationError = cv::calibrateCamera(calsets.object_point_set, calsets.image_point_set, cv::Size(640, 480), C.K, C.D, rvecs, tvecs);

    // Return
    return true;
};
