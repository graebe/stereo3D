#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <thread>
#include <future>
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
    void addCalibrationImages(std::vector<std::string> files);

private:
    CalibrationBoard board_;
    std::vector<std::string> calfiles_;
    CalibrationPointSet calsets_;
    std::vector<std::future<bool>> futures_;
    std::vector<bool> pattern_found_;
    void fillCheckerBoardObjectPoints(CalibrationPoints &calpoints);
    bool addCalibrationImage_(std::string file);
};
