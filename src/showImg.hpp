#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace ShowImg {

// Function to display an image given its path
int displayImage(const std::string& imagePath);
bool canOpenImage(const std::string& path);
} // namespace ShowImg
