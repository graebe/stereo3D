#include "calibration.hpp"

// Constructors
Calibrator::Calibrator(){};
Calibrator::Calibrator(CalibrationBoard board) : board_(board){};

// Helper Functions
void Calibrator::fillCheckerBoardObjectPoints_(CalibrationPoints &calpoints)
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

void Calibrator::drawChessboardCorners(cv::Mat image)
{
    auto calpoints = calsets_.object_point_set.back();
    auto pattern_found = pattern_found_.back();
    auto boardSize = board_.boardSize;
    cv::drawChessboardCorners(image, boardSize, calpoints, pattern_found);
}

bool Calibrator::findChessboardCorners_(CalibrationPoints &calpoints, cv::Mat &image)
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

bool Calibrator::addCalibrationImage_(std::string file)
{
    // Load Image
    cv::Mat image = loadColorImage(file);
    if (image.empty())
    {
        return false;
    }

    bool success = addCalibrationImage_(image);

    // Return
    return success;
}

bool Calibrator::addCalibrationImage_(cv::Mat image)
{
    CalibrationPoints calpoints;
    fillCheckerBoardObjectPoints_(calpoints);
    bool pattern_found = findChessboardCorners_(calpoints, image);
    if (pattern_found)
    {
        calsets_.object_point_set.emplace_back(calpoints.object_points);
        calsets_.image_point_set.emplace_back(calpoints.image_points);
        return true;
    }
    return false;
}

void Calibrator::addCalibrationImages(cv::Mat image) {
    bool result = addCalibrationImage_(image);
    pattern_found_.emplace_back(result);
}

void Calibrator::addCalibrationImages(std::vector<std::string> files)
{
    for (std::string file : files)
    {
        calfiles_.emplace_back(file);
        futures_.emplace_back(std::async(std::launch::async, [this, &file]()
                                         { return this->addCalibrationImage_(file); }));
    }

    for (int i = 0; i < futures_.size(); i++)
    {
        bool result = futures_[i].get();
        pattern_found_.emplace_back(result);
    }
}

void Calibrator::addCalibrationImages(std::vector<cv::Mat> images)
{
    for (cv::Mat image : images)
    {
        calfiles_.emplace_back("inmemory image");
        futures_.emplace_back(std::async(std::launch::async, [this, &image]()
                                         { return this->addCalibrationImage_(image); }));
    }

    for (int i = 0; i < futures_.size(); i++)
    {
        bool result = futures_[i].get();
        pattern_found_.emplace_back(result);
    }
}

int Calibrator::size() { return calfiles_.size(); }

// Calibrator
bool Calibrator::checkerboardCalibration(Camera *C)
{
    // Check number of images
    if (calfiles_.size() < 2)
    {
        std::cout << "Need a minimum of two images for calibration, currently it is: " << calfiles_.size() << std::endl;
        return false;
    }
    // Calibrate
    std::vector<cv::Mat> rvecs, tvecs;
    C->calibrationError = cv::calibrateCamera(calsets_.object_point_set, calsets_.image_point_set, C->imager.psize, C->K, C->D, rvecs, tvecs);
    return true;
    
}
bool Calibrator::checkerboardCalibration(Camera &C)
{
    // Check number of images
    if (calfiles_.size() < 2)
    {
        std::cout << "Need a minimum of two images for calibration, currently it is: " << calfiles_.size() << std::endl;
        return false;
    }
    // Calibrate
    std::vector<cv::Mat> rvecs, tvecs;
    C.calibrationError = cv::calibrateCamera(calsets_.object_point_set, calsets_.image_point_set, C.imager.psize, C.K, C.D, rvecs, tvecs);
    return true;
};

bool Calibrator::checkerboardCalibration(std::vector<std::string> files, Camera &C)
{
    addCalibrationImages(files);
    bool success = checkerboardCalibration(C);
    return success;
};

void Calibrator::printSummary()
{
    std::cout << "Calibration Summary" << std::endl;
    std::cout << "-------------------" << std::endl;
    for (int i = 0; i < calfiles_.size(); i++)
    {
        std::cout << "|-Image: " << calfiles_[i] << std::endl;
        std::cout << "| Pattern Found: " << pattern_found_[i] << std::endl;
        // std::cout << "| Coordinates: " << calsets_.image_point_set[i] << std::endl;
        std::cout << "|" << std::endl;
    }
}
