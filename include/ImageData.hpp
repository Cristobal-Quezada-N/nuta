#pragma once

#include <string>
#include <filesystem>
#include <opencv2/core/mat.hpp>

namespace fs = std::filesystem;

struct ImageData {
        const fs::path fpath;
        cv::Mat data;

        ImageData(const std::string &userInput);

        void read();
};
