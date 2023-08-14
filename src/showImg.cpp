// showImg.cpp

#include "showImg.h"
#include <iostream>

namespace ShowImg {

int displayImage(const std::string& imagePath) {
    // Load the image
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    
    // Check if the image was loaded successfully
    if(image.empty()) {
        std::cerr << "Error loading the image!" << std::endl;
        return -1;
    }

    // Create a window and display the image
    cv::namedWindow("Loaded Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Loaded Image", image);

    // Wait indefinitely until the user presses a key
    cv::waitKey(0);

    return 0;
}

bool canOpenImage(const std::string& path) {
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    return !image.empty();
}

} // namespace ShowImg
