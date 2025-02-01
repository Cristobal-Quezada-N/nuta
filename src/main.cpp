#include <iostream>
#include <optional>

#include <opencv2/highgui.hpp>

#include "ImageData.hpp"
#include "Barcode.hpp"

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

        } catch (const std::runtime_error& err) {
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
