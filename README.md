# stereo3D

![C++ CI](https://github.com/graebe/stereo3D/workflows/C++%20CI/badge.svg)

## Test Coverage Report
Click [Test Coverage Report](https://graebe.github.io/stereo3d/) to access the test coverage report.

## Compilation & Execution of the project

Install Prerequisite 1 - GTest
```
sudo apt-get update
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
```

Install Prerequisite 2 - 
```
sudo apt-get update
sudo apt-get install libncurses-dev
```

Compilation (in the project folder)
```
mkdir ./build
cd ./build
cmake ..
make
cd ..
```

Executioin (in the project folder) - On local computer with webcam
```
./build/calibration.bin --cam=0
```
(will show the same webcam image in two video streams purely for simulation of two cameras)


Execution (in the project folder) - On Jetson Nano
```
./build/calibration.bin --cam=jetson
```
(will show the camera stream of the IMX219-83)

To show images with opencv on the screen, you can use
```
./build/imshow.bin ./img_test_01.jpg
```

# Hardware

The project was implemented for the use of the following hardware:
## Jetson Nano
- [Youyeetou Developer Board](https://www.youyeetoo.com/products/subkit-nano-kit-new-nvidia-jetson-nano-b01-develop-kit-version-linux-demo-board-deep-learning-ai-development-board-platform?VariantsId=10564)
- [NVIDIA Board Documentation](https://developer.download.nvidia.com/assets/embedded/secure/jetson/Nano/docs/SP-09732-001_v1.1.pdf?c3FtYvv4EL6RsdZGTorQcMFfUhYwAfFfiXUZaUWWFAuoLkbt80nvOGVsobp9agFTPMDcqa_71oQHeeNjXiPsSNrlK63eZuTxqtPmF_ptI_7FtkGTR3v8gwr_8cWALD9OLs4K8apzEQwSI0uEICKq8wI5r79wnXyCsPPe7A==&t=eyJscyI6ImdzZW8iLCJsc2QiOiJodHRwczovL3d3dy5nb29nbGUuY29tLyJ9)
## Stereo Camera
- [Camera Hardware](https://www.waveshare.com/wiki/IMX219-83_Stereo_Camera)
- [Datasheet ICM 20948](https://www.waveshare.com/w/upload/1/18/DS-000189-ICM-20948-v1.3.pdf)


# Udacity

Chosen Project:
Custom project with an open cv calibration process for a stereo camera (jetson nano).
## Udacity Checklist

README (All Rubric Points REQUIRED)
| Success Criteria | Fulfillment |
|------------------|-------------|
| A Readme with instructions is included within the project | This file. |
| The Readme indicates which project is chosen | See above. |
| The Readme incluces information about each rubric point addressed | See below. |

Compiling and Testing (All Rubric Points REQUIRED)
| Success Criteria | Fulfillment |
|------------------|-------------|
| The submission must compile and run | Runs on jetson nano. Alternative: Webcam ob computer. See documentation above. |

Loops, Functions, I/O
| Success Criteria | Fulfillment |
|------------------|-------------|
| The submission must compile and run | Runs on jetson nano. Alternative: Webcam ob computer. See documentation above. |
| The project reads data from a file and process the data, or the program writes data to a file. | Loading images in method Calibrator::addCalibrationImages(std::vector\<std::string\> files), saving files in void Camera::saveImage(std::string filename). |
| The project accepts user input and processes the input. | During calibration (see execution of calibration.bin above), the user can press space to capture a camera frame for calibration or q to exit. |

Object Oriented Programming
| Success Criteria | Fulfillment |
|------------------|-------------|
| The project uses Object Oriented Programming techniques. | See camera.cpp and calibration.cpp. |
| Classes use appropriate access specifiers for class members. | See camera.cpp and calibration.cpp. |
| Class constructors utilize member initialization lists. | See Camera::Camera(cv::Mat K_, cv::Mat D_) : K(K_), D(D_){}; |
| Classes encapsulte behavior. | See camera.cpp and calibration.cpp. |
| Classes follow an appropriate inheritance hierarchy. | See interface definitions for Camera and MultiCamera. |
| Overloaded functions allow the same function to operate on different parameters. | See e.g. void Calibrator::addCalibrationImages(...) |
| Derived class functions override virtual base class functions. | See interface definitions for Camera and MultiCamera and their inheritance. |
| Templates generalize functions in the project | I have implemented an example, as proof of knowledge in image.cpp. |

Memory Management
| Success Criteria | Fulfillment |
|------------------|-------------|
| The project makes use of references in function declarations. | See Calibrator::checkerboardCalibration(...). |
| The project uses destructuors appropriately. | See camera, destructor releases the capture stream of the camera. Further ressources do not need to be freed, since the image is owned with a unique_ptr. |
| The project follows the rule of 5. | Implemented for Camera and Calibrator. |
| The project uses move semantics to move data, instead of copying it, where possible. | See int Camera::capture(). |
| The project uses smart pointers instead of raw pointers. | See int Camera::capture() and Camera properties. The camera object owns the captured image. |

Concurrency
| Success Criteria | Fulfillment |
|------------------|-------------|
| The project uses multithreading. | See Calibrator::addCalibrationImages(std::vector\<std::string\> files). |
| A promise and future is used in the project. | See Calibrator::addCalibrationImages(std::vector\<std::string\> files). |
| A mutex or lock is used in the project. | See Calibrator. |
| A condition variable is used in the project. | See implementation of main_multitask.cpp. |
