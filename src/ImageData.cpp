#include <iostream>
#include <string>
#include <stdexcept>

#include <opencv2/imgcodecs.hpp>

#include "ImageData.hpp"

ImageData::ImageData(const std::string &userInput) : fpath(userInput) {
        if (!fs::exists(fpath)) {
                throw  std::runtime_error("File path not found: " + fpath.string());
        }
        std::cout << "Image: " << fpath << std::endl;
}

void ImageData::read() {
        data = cv::imread(fpath);
        if (data.empty()) {
                throw  std::runtime_error("Could not read the image: " + fpath.string());
        }
}
