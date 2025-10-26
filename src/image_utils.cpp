#include "image_utils.h"

// Video parameters
VideoInfo getVideoInfo(cv::VideoCapture& cap) {
	VideoInfo info;
	info.w = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	info.h = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	info.fps = cap.get(cv::CAP_PROP_FPS);
	info.numFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
	info.framePeriod = static_cast<int>(1000.0 / info.fps); // ms
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


