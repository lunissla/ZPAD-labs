#include "FaceDetector.hpp"

FaceDetector::FaceDetector(const std::string& proto, const std::string& model)
    : stopFlag(false)
{
    net = cv::dnn::readNetFromCaffe(proto, model);
    worker = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    stopFlag = true;
    frameCond.notify_one();
    if (worker.joinable()) worker.join();
}

void FaceDetector::setFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(frameMutex);
    frame.copyTo(lastFrame);
    newFrameAvailable = true;
    frameCond.notify_one();
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(facesMutex);
    return faces;
}

void FaceDetector::workerLoop() {
    while (!stopFlag) {
        cv::Mat frame;

        {
            std::unique_lock<std::mutex> lock(frameMutex);
            frameCond.wait(lock, [&]{
                return newFrameAvailable || stopFlag;
            });

            if (stopFlag) return;

            lastFrame.copyTo(frame);
            newFrameAvailable = false;
        }

        if (frame.empty()) continue;

        // OPTIONAL: штучне навантаження
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        cv::Mat blob = cv::dnn::blobFromImage(
            frame, 1.0, cv::Size(300, 300),
            cv::Scalar(104, 177, 123), false, false
        );

        net.setInput(blob);
        cv::Mat det = net.forward();

        std::vector<cv::Rect> found;
        int w = frame.cols;
        int h = frame.rows;

        cv::Mat detMat(det.size[2], det.size[3], CV_32F, det.ptr<float>());

        for (int i = 0; i < detMat.rows; i++) {
            float conf = detMat.at<float>(i, 2);
            if (conf < 0.5f) continue;

            int x1 = detMat.at<float>(i, 3) * w;
            int y1 = detMat.at<float>(i, 4) * h;
            int x2 = detMat.at<float>(i, 5) * w;
            int y2 = detMat.at<float>(i, 6) * h;

            found.emplace_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
        }

        {
            std::lock_guard<std::mutex> lock(facesMutex);
            faces = std::move(found);
        }
    }
}
