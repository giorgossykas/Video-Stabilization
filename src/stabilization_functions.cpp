#include "stabilization_functions.h"
#include "image_utils.h"
#include <iostream>
#include <cmath>

// Compute transforms for each frame pair with ORB + Affine Matrix
void estimateMotionTransforms(cv::VideoCapture& cap, 
							  VideoInfo& info,
							  std::vector<cv::Mat>& transforms)
{
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
		std::vector<cv::Point2f> good_prev, good_curr;
		matcher.match(des1, des2, matches);
		// Sort matches by distance (ascending)
		std::sort(matches.begin(), matches.end(),
			[](const cv::DMatch& a, const cv::DMatch& b) {
				return a.distance < b.distance;
			});
		// Keep only the best 100 (or fewer if not enough)
		int numGoodMatches = std::min(100, static_cast<int>(matches.size()));
		std::vector<cv::DMatch> good_matches(matches.begin(),
											matches.begin() + numGoodMatches);
		for (auto& m : good_matches) {
			good_prev.push_back(kp1[m.queryIdx].pt);
			good_curr.push_back(kp2[m.trainIdx].pt);
		}

		// Get affine matrix of frame pair
		cv::Mat H = cv::estimateAffinePartial2D(good_prev, good_curr);
		if (H.empty()) H = cv::Mat::eye(2, 3, CV_64F);
		// Add to transforms vector
		transforms.push_back(H);
		// Update previous frame
		prevGray = currGray.clone();
		// If I don't use .clone(), then if I later change 
		// currGray(which I will), prevGray will change too!

	}
}

// Accumulate transform from each frame pair to get trajectory
void accumulateTrajectory(const std::vector<cv::Mat>& transforms,
						  std::vector<cv::Mat>& trajectory) {

	cv::Mat T = cv::Mat::eye(2, 3, CV_64F);
	for (auto& H : transforms) {
		cv::Mat H3x3 = cv::Mat::eye(3, 3, CV_64F);
		H.copyTo(H3x3(cv::Rect(0, 0, 3, 2)));

		cv::Mat T3x3 = cv::Mat::eye(3, 3, CV_64F);
		T.copyTo(T3x3(cv::Rect(0, 0, 3, 2)));

		T3x3 = H3x3 * T3x3;

		cv::Mat T2x3 = T3x3(cv::Rect(0, 0, 3, 2)).clone();
		trajectory.push_back(T2x3);
		T = T2x3;
	}
}

// Smooth trajectory from past and feature frames/matrices
void smoothTrajectory(const std::vector<cv::Mat>& trajectory,
					  std::vector<cv::Mat>& smoothedTrajectory, int radius = 5) {

	int N = trajectory.size();
	for (int i = 0; i < N; ++i) {
		double sum_dx = 0, sum_dy = 0, sum_da = 0;
		int count = 0;
		for (int j = -radius; j <= radius; ++j) {
			int idx = std::min(std::max(i + j, 0), N - 1); // Keeps it in [0, N-1]
			double dx = trajectory[idx].at<double>(0, 2);
			double dy = trajectory[idx].at<double>(1, 2);
			double da = std::atan2(trajectory[idx].at<double>(1, 0),
								   trajectory[idx].at<double>(0, 0));
			sum_dx += dx; sum_dy += dy; sum_da += da;
			count++;
		}

		double avg_dx = sum_dx / count;
		double avg_dy = sum_dy / count;
		double avg_da = sum_da / count;
		
		cv::Mat T = (cv::Mat_<double>(2, 3) <<
			std::cos(avg_da), -std::sin(avg_da), avg_dx,
			std::sin(avg_da),  std::cos(avg_da), avg_dy);

		smoothedTrajectory.push_back(T);
	}
}

// Apply stabilization
void applyStabilization(cv::VideoCapture& cap,
						VideoInfo& info,
						const std::vector<cv::Mat>& trajectory,
						std::vector<cv::Mat>& smoothedTrajectory,
						const std::string& outputFile) {
	
	// Start from the first frame again because it was already opened
	cap.set(cv::CAP_PROP_POS_FRAMES, 0);

	// Video writer
	cv::VideoWriter writer(outputFile,
		cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
		info.fps,
		cv::Size(info.w, info.h));

	// Read first frame
	cv::Mat currFrame, currGray;
	if (readFrame(cap, currFrame, currGray)) return; // runs the readFrame
	writer.write(currFrame);

	for (size_t i = 0; i <= smoothedTrajectory.size(); ++i) {
		if (readFrame(cap, currFrame, currGray)) break;

		// Compute delta
		cv::Mat T_orig = trajectory[i];
		cv::Mat T_smoothed = smoothedTrajectory[i];

		// Smooth - Original returns jitter/high frequency
		double dx = T_smoothed.at<double>(0, 2) - T_orig.at<double>(0, 2);
		double dy = T_smoothed.at<double>(1, 2) - T_orig.at<double>(1, 2);
		double da = std::atan2(T_smoothed.at<double>(1, 0), T_smoothed.at<double>(0, 0)) -
					std::atan2(T_orig.at<double>(1, 0), T_orig.at<double>(0, 0));
		cv::Mat T_delta = (cv::Mat_<double>(2, 3) <<
			std::cos(da), -std::sin(da), dx,
			std::sin(da),  std::cos(da), dy);

		// Warp
		cv::Mat stabFrame;
		cv::warpAffine(currFrame, stabFrame, T_delta, currFrame.size());
		writer.write(stabFrame);

	}
	writer.release();
}