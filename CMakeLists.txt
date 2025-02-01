cmake_minimum_required(VERSION 3.15...3.31)
project(Nuta LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV: ${OpenCV_VERSION}")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/ImageData.cpp
        ${SRC_DIR}/Barcode.cpp
)

add_executable(nuta ${SOURCES})

target_include_directories(nuta PRIVATE ${INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(nuta PRIVATE ${OpenCV_LIBS})
