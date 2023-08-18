#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "camera.hpp"

struct CalibrationBoard
{
    CalibrationBoard() {}
    CalibrationBoard(int width, int height, float sqSize)
        : boardSize(width, height), sqSize(sqSize) {}
    cv::Size_<int> boardSize;
    float sqSize;
};

class Calibrator
{
public:
    Calibrator();
    Calibrator(CalibrationBoard board);
    bool checkerboardCalibration(const std::vector<std::string> files, Camera &C);

private:
    CalibrationBoard board_;
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;
    std::vector<cv::Point3f> obj_;
    std::vector<cv::Point2f> corners_;

    void initObjPoints();
};
