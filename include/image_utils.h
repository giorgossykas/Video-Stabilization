#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <opencv2/opencv.hpp>

// Parameters of Video
struct VideoInfo {
	int w; // width
	int h; // height
	double fps;
	int numFrames;
	int framePeriod;
};

VideoInfo getVideoInfo(cv::VideoCapture& cap);


// Reads frame and stores color and grayscale cv::Mat while checking validity
bool readFrame(cv::VideoCapture& cap, cv::Mat& frame, cv::Mat& gray);


// Add delay for live video display
/*
auto start = std::chrono::high_resolution_clock::now();
// Add delay for fps
auto end = std::chrono::high_resolution_clock::now();
auto processTime =
    std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
// Compute remaining delay to maintain target fps
int delay_ms = std::max(1, framePeriod - static_cast<int>(processTime));
// Wait for the computed delay (also handles imshow() window events)
int key = cv::waitKey(delay_ms);
if (key == 27) break; // Esc to exit
*/


#endif