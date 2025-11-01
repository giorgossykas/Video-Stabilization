// Main
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <chrono>
#include <thread>
#include <vector>

// OpenCV
#include <opencv2/opencv.hpp>

// Mine
#include "include/image_utils.h"
#include "include/stabilization_functions.h"

int main(int argc, char** argv)
{
    // IO files
    std::string filename = "resources/videos/shaky_car.mp4";
    std::string stab_vid = "resources/videos/stabilized_output.mp4";

    // Open video
    cv::VideoCapture cap(filename);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file!" << std::endl;
        return 1;
    }

    // Video Parameters,
    VideoInfo vidInfo = getVideoInfo(cap);

    // Start processing

    std::cout << "\nStarting processing\n" << std::endl;

    // Get vector of all frame-pair affine transforms
    std::vector<cv::Mat> transforms;
    estimateMotionTransforms(cap, vidInfo, transforms);

    std::cout << "Estimated motion transforms\n" << std::endl;

    // Accumulate transforms
    std::vector<cv::Mat> trajectory;
    accumulateTrajectory(transforms, trajectory);

    std::cout << "Accumulated transforms\n" << std::endl;

    // Smooth trajectory
    std::vector<cv::Mat> smoothedTrajectory;
    smoothTrajectory(trajectory, smoothedTrajectory, 5);

    std::cout << "Smoothed trajectory\n" << std::endl;

    // Apply stabilization
    applyStabilization(cap, vidInfo, trajectory, smoothedTrajectory, stab_vid);

    std::cout << "Applied stabilization\n" << std::endl;

    return 0;
}

