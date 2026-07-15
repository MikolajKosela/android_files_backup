sudo apt update
sudo apt install qt6-base-dev cmake ninja-build g++ adb
cmake -S . -B build -G Ninja
cmake --build build