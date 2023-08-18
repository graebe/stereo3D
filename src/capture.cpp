#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
    cv::VideoCapture cap(0); // open the default camera

    if (!cap.isOpened())
    { // check if the camera opened successfully
        std::cerr << "Error: Couldn't open the webcam." << std::endl;
        return -1;
    }

    cv::Mat frame;

    // Allow the camera to warm up
    int warmUpFrames = 30;
    for (int i = 0; i < warmUpFrames; ++i)
    {
        cap >> frame;
        cv::waitKey(30); // wait for a short time (30 ms)
    }

    // Now, capture a frame
    cap >> frame;

    if (frame.empty())
    {
        std::cerr << "Error: Captured frame is empty." << std::endl;
        return -1;
    }

    cv::imwrite("captured_image.jpg", frame); // Save the captured frame as JPEG

    cap.release(); // Release the VideoCapture object

    std::cout << "Image saved to 'captured_image.jpg'" << std::endl;
    return 0;
}
