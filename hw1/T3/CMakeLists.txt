cmake_minimum_required(VERSION 3.10)
project(demoo)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV 4 REQUIRED)

add_executable(demoo film.cpp)
target_link_libraries(demoo ${OpenCV_LIBS})
