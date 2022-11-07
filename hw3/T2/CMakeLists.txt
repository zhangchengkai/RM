cmake_minimum_required(VERSION 3.10)
project(demo)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 4 REQUIRED)
find_package (Eigen3 REQUIRED NO_MODULE)

add_executable(demo main.cpp)
target_link_libraries(demo ${OpenCV_LIBS})
target_link_libraries ( demo Eigen3::Eigen )