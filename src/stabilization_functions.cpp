#include "stabilization_functions.h"
#include "image_utils.h"
#include <iostream>

// Compute transforms for each frame pair
void estimateMotionTransforms(cv::VideoCapture& cap, 
							std::vector<cv::Mat>& transforms,
							VideoInfo& info) {
	// Initialize detector and matcher
	cv::Ptr<cv::ORB> orb = cv::ORB::create(1000);
	cv::BFMatcher matcher(cv::NORM_HAMMING, true);

	// Get frames
	cv::Mat prevFrame, prevGray;
	if (!readFrame(cap, prevFrame, prevGray)) { // runs the readFrame
	std::cerr << "Could not load frame" << std::endl;
	}
	for (int i = 1; i < info.numFrames; ++i) {
		cv::Mat currFrame, currGray;
		if (readFrame(cap, currFrame, currGray)) break; // runs the readFrame

		// Initialize and compute keypoints and descriptors
		std::vector<cv::KeyPoint> kp1, kp2;
		cv::Mat des1, des2;
		orb->detectAndCompute(prevGray, cv::noArray(), kp1, des1);
		orb->detectAndCompute(currGray, cv::noArray(), kp2, des2);
		// Match and keep good points
		std::vector<cv::DMatch> matches;
		std::vector<cv::Point2f> pts1, pts2;
		matcher.match(des1, des2, matches);
		for (auto& m : matches) {
			pts1.push_back(kp1[m.queryIdx].pt);
			pts2.push_back(kp2[m.trainIdx].pt);
		}
		// Get affine matrix of frame pair
		cv::Mat H = cv::estimateAffinePartial2D(pts1, pts2);
		if (H.empty()) H = cv::Mat::eye(2, 3, CV_64F);
		// Add to transforms vector
		transforms.push_back(H);
		// Update previous frame
		prevGray = currGray.clone();
		// If I don't use .clone(), then if I later change 
		// currGray(which I will), prevGray will change too!

	}
}