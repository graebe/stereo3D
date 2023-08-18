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

struct CalibrationPoints
{
    std::vector<cv::Point3f> object_points;
    std::vector<cv::Point2f> image_points;
};

struct CalibrationPointSet
{
    std::vector<std::vector<cv::Point3f>> object_point_set;
    std::vector<std::vector<cv::Point2f>> image_point_set;
};

class Calibrator
{
public:
    Calibrator();
    Calibrator(CalibrationBoard board);
    bool checkerboardCalibration(const std::vector<std::string> files, Camera &C);
    bool findChessboardCorners(CalibrationPoints &calpoints, cv::Mat &image);
    bool addCalibrationImage(std::string file);

private:
    CalibrationBoard board_;
    CalibrationPointSet calsets;
    void fillCheckerBoardObjectPoints(CalibrationPoints &calset);
};
