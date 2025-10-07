#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <opencv2/opencv.hpp>

cv::Mat sobel(const cv::Mat& img); //const means that this function won’t alter the input image.

#endif