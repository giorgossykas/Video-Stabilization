#include "image_utils.h"

cv::Mat sobel(const cv::Mat& img) {
	cv::Mat img_sobel;
	cv::cvtColor(img, img_sobel, cv::COLOR_BGR2GRAY);
	return img_sobel;
}



