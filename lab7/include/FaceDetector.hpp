#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

class FaceDetector {
public:
    FaceDetector(const std::string& proto, const std::string& model);
    ~FaceDetector();

    void setFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getFaces();

private:
    void workerLoop();

    cv::dnn::Net net;

    std::thread worker;
    std::atomic<bool> stopFlag;

    cv::Mat lastFrame;
    bool newFrameAvailable = false;

    std::mutex frameMutex;
    std::condition_variable frameCond;

    std::vector<cv::Rect> faces;
    std::mutex facesMutex;
};
