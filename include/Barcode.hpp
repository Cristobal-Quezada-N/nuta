#pragma once

#include <vector>
#include <string>

#include <opencv2/core/mat.hpp>
#include <opencv2/objdetect/barcode.hpp>

#include "ImageData.hpp"

struct Barcode {
        private:
                cv::Mat corners; // [N][4][2]
                std::vector<std::string> info;
                std::vector<std::string> format;
                const ImageData &srcImage;

        public:
                Barcode(const ImageData &image) : srcImage(image) {}

                const cv::Mat                   getCorners()      const { return corners; }
                const std::vector<std::string>  getInfo()         const { return info;    }
                const std::vector<std::string>  getFormat()       const { return format;  }

                void decode();
                void drawArea();
                void display();

                static Barcode decodeFromImage(const ImageData &image);
};
