#include "image_utils.h"
#include <iostream>

// Video parameters
VideoInfo getVideoInfo(cv::VideoCapture& cap) {
	VideoInfo info{};
	info.w = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	info.h = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	info.fps = cap.get(cv::CAP_PROP_FPS);
	info.numFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
	if (info.fps > 0.0)
		info.framePeriod = static_cast<int>(1000.0 / info.fps); // ms
	else
		info.framePeriod = 33; // Default value for 30 fps.

	return info;
}

// Read frame and return frame and grayscale
bool readFrame(cv::VideoCapture& cap, cv::Mat& frame, cv::Mat& gray) {
	cap >> frame;
	if (frame.empty()) {
		return false;
	}
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	return true;
}

// Display with correct frame rate


