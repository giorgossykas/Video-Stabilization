
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

    // Detect edges, Canny and Sobel
    cv::Mat cannyEdges;
    cv::Canny(blurred, cannyEdges, 100, 200);
    cv::Mat grad_x, abs_grad_x, grad_y, abs_grad_y, sobelEdges;
    cv::Sobel(blurred, grad_x, CV_16S, 1, 0, 3);
    cv::Sobel(blurred, grad_y, CV_16S, 0, 1, 3);
    //cv::magnitude(grad_x, grad_y, sobelEdges);
    //cv::normalize(sobelEdges, sobelEdges, 0, 255, cv::NORM_MINMAX);
    //sobelEdges.convertTo(sobelEdges, CV_8U);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobelEdges);
    // addWeighted is good for display, needs CV_16S
    // magnitude is good for numerical accuracy, needs CV_32F


    // Display the images
    cv::imshow("Original Image", img);
    cv::imshow("Grayscale", gray);
    cv::imshow("Blurred", blurred);
    cv::imshow("Canny Edges", cannyEdges);
    cv::imshow("Sobel Edges", sobelEdges);

    // Wait until any kery is pressed
    cv::waitKey(0);

    return 0;
}

