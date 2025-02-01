#include <iostream>
#include <stdexcept>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/cvstd.hpp>

#include "ImageData.hpp"
#include "Barcode.hpp"

Barcode Barcode::decodeFromImage(const ImageData &image) {
        Barcode newBarcode(image);
        newBarcode.decode();
        return newBarcode;
}

void Barcode::display() {
        for (int i = 0; i < info.size(); i++) {
                std::cout << "Barcode "         << i + 1 << ":" << std::endl;
                std::cout << "\tInfo: "         << info[i]      << std::endl;
                std::cout << "\tFormat: "       << format[i]    << std::endl; 
        }
}

void Barcode::decode() {
        cv::Ptr<cv::barcode::BarcodeDetector> barcodeDetector = cv::makePtr<cv::barcode::BarcodeDetector>();
        bool barcodeDetected = barcodeDetector->detect(srcImage.data, corners);

        if (!barcodeDetected) {
                throw  std::runtime_error("Could not detect barcode: " + (srcImage.fpath).string());
        }

        bool barDecode = barcodeDetector->decodeWithType(
                srcImage.data,
                corners,
                info,
                format
        );

        if (!barDecode) {
                throw  std::runtime_error("Could not decode barcode: " + (srcImage.fpath).string());
        }
}

void Barcode::drawArea(){
        // Draw the detected barcode regions and display decoded text
        for (int i = 0; i < corners.rows; i++) {
                cv::Mat corner = corners.row(i).reshape(2, 4);

                std::vector<cv::Point> points;
                for (int j = 0; j < 4; j++) {
                        points.push_back(cv::Point(
                                corner.at<float>(j, 0),
                                corner.at<float>(j, 1)
                        ));
                }

                // Draw the bounding box
                cv::polylines(srcImage.data, points, true, cv::Scalar(0, 255, 0), 2);

                if (!info.empty()) {
                        cv::Point text_position(points[0].x, points[0].y - 10);
                        cv::putText(
                                srcImage.data,
                                info[i],
                                text_position,
                                cv::FONT_HERSHEY_SIMPLEX,
                                0.8,
                                cv::Scalar(0, 0, 255),
                                2
                        );
                }
        }
}

