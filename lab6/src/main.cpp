#include <opencv2/opencv.hpp>

#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"

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

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) continue;

        cv::Mat processed = processor.process(frame, mode);
        display.show(processed);

        int key = cv::waitKey(1);

        if (key == 27) break; // ESC
        mode = keyProcessor.process(key);
    }

    return 0;
}