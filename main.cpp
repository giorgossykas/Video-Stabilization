
#include <iostream>
#include <opencv2//opencv.hpp>

int main()
{

    // Load image
    cv::Mat img = cv::imread("resources/images/car_rgb.jpg");

    // Check if image was loaded correctly
    if (img.empty()) {
        std::cerr << "Error: Could not open or find image" << std::endl;
        return -1;
    }

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Apply Gaussian blur
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);

    // Display the images
    cv::imshow("Original Image", img);
    cv::imshow("Grayscale", gray);
    cv::imshow("Blurred", blurred);

    // Wait until any kery is pressed
    cv::waitKey(0);

    return 0;
}

