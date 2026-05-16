#pragma once
#include <opencv2/opencv.hpp>

class Display {
public:
    void show(const cv::Mat& frame);
};