// Main
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <chrono>
#include <thread>

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
    int framePeriod = static_cast<int>(1000.0 / fps); // ms
    std::cout << "Target FPS: " << fps << " (" << framePeriod << " ms per frame)\n";

    // Video writer
    /*
    cv::VideoWriter writer(stab_vid,
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        fps,
        cv::Size(w, h));*/

    // Read and Display frames
    for (int i = 0; i < numFrames; i++) {

        auto start = std::chrono::high_resolution_clock::now();

        cv::Mat currFrame, currGray;
        cap >> currFrame;
        if (currFrame.empty()) break;
        cv::cvtColor(currFrame, currGray, cv::COLOR_BGR2GRAY);
        cv::imshow("Shaky Car", currFrame);

        // Add delay for fps
        auto end = std::chrono::high_resolution_clock::now();
        auto processTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        // Compute remaining delay to maintain target fps
        int delay_ms = std::max(1, framePeriod - static_cast<int>(processTime));
        std::cout << delay_ms << std::endl;
        // Wait for the computed delay (also handles imshow() window events)
        int key = cv::waitKey(1);
        //if (key == 27) break; // Esc to exit

    }


    cap.release();
    //writer.release();
    cv::destroyAllWindows();

    return 0;
}

