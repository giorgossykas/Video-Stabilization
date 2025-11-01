#ifndef STABILIZATION_FUNCTIONS_H
#define STABILIZATION_FUNCTIONS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "image_utils.h"

// Estimate per-frame motion
void estimateMotionTransforms(cv::VideoCapture& cap, 
	VideoInfo& info, std::vector<cv::Mat>& transfomrs);

// Accumulate Trajectory
void accumulateTrajectory(const std::vector<cv::Mat>& transforms,
	std::vector<cv::Mat>& trajectory);

// Smooth trajectory
void smoothTrajectory(const std::vector<cv::Mat>& trajectory,
	std::vector<cv::Mat>& smoothedTrajectory, int radius = 5);

// Apply stabilization
void applyStabilization(cv::VideoCapture& cap,
	VideoInfo& info,
	const std::vector<cv::Mat>& trajectory,
	std::vector<cv::Mat>& smoothedTrajectory,
	const std::string& outputFile);

#endif