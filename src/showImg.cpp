#include "showImg.hpp"
#include <iostream>

namespace ShowImg
{

    int displayImage(const std::string &imagePath)
    {
        cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
        if (image.empty())
        {
            std::cerr << "Error loading the image!" << std::endl;
            return -1;
        }
        cv::namedWindow("Loaded Image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Loaded Image", image);
        cv::waitKey(0);
        return 0;
    }

    bool canOpenImage(const std::string &path)
    {
        cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
        return !image.empty();
    }

}
