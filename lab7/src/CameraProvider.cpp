#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int index) {
    cap.open(index);
}

bool CameraProvider::isOpened() {
    return cap.isOpened();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}