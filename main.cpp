// Main
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

// OpenCV
#include <opencv2/opencv.hpp>

int main(int argc, char** argv)
{
    // IO files
    std::string filename = "resources/videos/shaky_car.mp4";
    std::string stab_vid = "resources/videos/stabilized_output.mp4";

    // Read video
    cv::VideoCapture cap(filename);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file!" << std::endl;
        return -1;
    }

    // Parameters
    int w = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int h = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);
    int numFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));

    // Video writer
    /*
    cv::VideoWriter writer(stab_vid,
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        fps,
        cv::Size(w, h));*/

    // Read and Display frames
    for (int i = 0; i < numFrames; i++) {
        cv::Mat currFrame, currGray;
        cap >> currFrame;
        if (currFrame.empty()) break;
        cv::cvtColor(currFrame, currGray, cv::COLOR_BGR2GRAY);
        cv::imshow("Shaky Car", currFrame);
        if (cv::waitKey(1) == 27) break; // press 'ESC' to exit
    }


    cap.release();
    //writer.release();
    cv::destroyAllWindows();

    return 0;
}

