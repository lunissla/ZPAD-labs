#include <opencv2/opencv.hpp>

#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"

int main() {
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cout << "Camera not opened\n";
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor processor;
    Display display;

    KeyProcessor::Mode mode = KeyProcessor::ORIGINAL;

    FaceDetector faceDetector(
    "models/deploy.prototxt",
    "models/res10_300x300_ssd_iter_140000.caffemodel"
    );

    bool faceMode = false;


    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) continue;

        if (faceMode) {
            faceDetector.setFrame(frame);

            auto faces = faceDetector.getFaces();
            for (auto& r : faces) {
                cv::rectangle(frame, r, cv::Scalar(0,255,0), 2);
            }
        }  

        cv::Mat processed = processor.process(frame, mode);
        display.show(processed);

        int key = cv::waitKey(1);
        if (key == 27) break;

        if (key == 'f' || key == 'F')
            faceMode = !faceMode;

        mode = keyProcessor.process(key);

    }

    return 0;
}