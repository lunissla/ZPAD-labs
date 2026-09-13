#include "FrameProcessor.hpp"

cv::Mat FrameProcessor::process(const cv::Mat& frame, KeyProcessor::Mode mode) {
    cv::Mat out;

    switch (mode) {
        case KeyProcessor::ORIGINAL:
            out = frame.clone();
            break;

        case KeyProcessor::GRAY:
            cv::cvtColor(frame, out, cv::COLOR_BGR2GRAY);
            cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
            break;

        case KeyProcessor::BLUR:
            cv::GaussianBlur(frame, out, cv::Size(15, 15), 0);
            break;

        case KeyProcessor::CANNY: {
            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, out, 50, 150);
            cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
            break;
        }

        case KeyProcessor::INVERT:
            cv::bitwise_not(frame, out);
            break;

        case KeyProcessor::SOBEL: {
            cv::Mat gray, grad_x;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, grad_x, CV_8U, 1, 0);
            cv::cvtColor(grad_x, out, cv::COLOR_GRAY2BGR);
            break;
        }
    }

    return out;
}