#include <iostream>
#include <filesystem>
#include <optional>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/barcode.hpp>

namespace fs = std::filesystem;

struct ImageData {
        const fs::path fpath;
        cv::Mat data;

        ImageData(const std::string &userInput) : fpath(userInput){
                if (!fs::exists(fpath)){
                        throw  std::runtime_error("File path not found: " + fpath.string());
                }
                std::cout << "Image: " << fpath << std::endl;
        }

        void read(){
                data = cv::imread(fpath);
                if (data.empty()) {
                        throw  std::runtime_error("Could not read the image: " + fpath.string());
                }
        }
};

struct Barcode {
        private:
                cv::Mat corners; // [N][4][2]
                std::vector<std::string> info;
                std::vector<std::string> format;
                const ImageData &srcImage;

        public:
                const cv::Mat                   getCorners()      const { return corners; }
                const std::vector<std::string>  getInfo()         const { return info;    }
                const std::vector<std::string>  getFormat()       const { return format;  }

                Barcode(const ImageData &image) : srcImage(image) {}

                void decode();

                static Barcode decodeFromImage(const ImageData &image){
                        Barcode newBarcode(image);
                        newBarcode.decode();
                        return newBarcode;
                }

                void display(){
                        for (size_t i = 0; i < info.size(); i++) {
                                std::cout << "Barcode "         << i + 1 << ":" << std::endl;
                                std::cout << "\tInfo: "         << info[i]      << std::endl;
                                std::cout << "\tFormat: "       << format[i]    << std::endl; 
                        }

                }
                void drawArea();
};

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

int main(int argc, char *argv[]) {
        // Check program usage
        if (argc != 2) {
                std::cerr << "Error. Invalid use of command"            << std::endl;
                std::cout << "Usage: " << argv[0] << " <image_path>"    << std::endl;
                exit(1);
        }

        // Barcode Detection
        std::optional<ImageData> image;
        std::optional<Barcode> barcode;
        try {
                // Read Image
                image.emplace(argv[1]);
                image->read();

                // Detect Barcode
                barcode.emplace(Barcode::decodeFromImage(image.value()));
                barcode->display();
                barcode->drawArea();

        } catch (const std::runtime_error& err){
                std::cerr << err.what() << std::endl;
                return 1;
        }

        // Window setup
        cv::imshow("Display window", image.value().data);
        int keyStroke = cv::waitKey(0);
        if (keyStroke > 0) {
                cv::destroyAllWindows();
                exit(1);
        }

        return 0;
}
