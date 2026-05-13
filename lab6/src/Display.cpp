#include "Display.hpp"

void Display::show(const cv::Mat& frame) {
    cv::imshow("Lab 6 Camera", frame);
}