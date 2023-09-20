# Compile GTest
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib

# Back to Project Dir
cd /home/workspace/stereo3d

# Install Dependencies
sudo apt-get update
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get install libncurses-dev
sudo apt-get install g++-9
