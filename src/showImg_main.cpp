#include "showImg.h"
#include <iostream>

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_image>" << std::endl;
        return -1;
    }
    
    return ShowImg::displayImage(argv[1]);
}
