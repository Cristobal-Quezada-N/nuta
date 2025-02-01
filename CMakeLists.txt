cmake_minimum_required(VERSION 3.15...3.31)
project(Nuta LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

find_package(OpenCV REQUIRED COMPONENTS imgproc imgcodecs highgui objdetect)
message(STATUS "OpenCV: ${OpenCV_VERSION}")

add_executable(nuta main.cpp)
target_link_libraries(nuta PRIVATE ${OpenCV_LIBS})
target_include_directories(nuta PRIVATE ${OpenCV_INCLUDE_DIRS})
