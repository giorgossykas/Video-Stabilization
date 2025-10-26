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

    // ORB detector
    cv::Ptr<cv::Feature2D> detector = cv::ORB::create(1000); // up to 1000 keypoints
    
    // BF Marcher
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    
    // Video writer
    cv::VideoWriter writer(stab_vid,
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        vidInfo.fps,
        cv::Size(vidInfo.w, vidInfo.h));
    

    // Get first frame
    cv::Mat prevFrame, prevGray;
    if (!readFrame(cap, prevFrame, prevGray)) {
        std::cerr << "Error: cannot read first frame" << std::endl;
        return 1;
    }

    // Read and Display frames, start from 2nd frame (i==1)
    for (int i = 1; i < vidInfo.numFrames; i++) {






        cv::Mat currFrame, currGray;
        cap >> currFrame;
        if (prevFrame.empty()) {
            std::cerr << "Error loading images" << std::endl;
            return 1;
        }
        cv::cvtColor(currFrame, currGray, cv::COLOR_BGR2GRAY);

        // 1. Detect keypoints and descriptors
        std::vector<cv::KeyPoint> kp1, kp2;
        cv::Mat des1, des2;
        detector->detectAndCompute(prevGray, cv::noArray(), kp1, des1);
        detector->detectAndCompute(currGray, cv::noArray(), kp2, des2);

        // 2. Match descriptors using BFMatcher
        std::vector<std::vector<cv::DMatch>> knnMatches;
        matcher.knnMatch(des1, des2, knnMatches, 2);

        










        cv::imshow("Shaky Car", currFrame);

    }


    cap.release();
    //writer.release();
    cv::destroyAllWindows();

    return 0;
}

