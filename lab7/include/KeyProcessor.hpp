#pragma once

class KeyProcessor {
public:
    enum Mode {
        ORIGINAL = 0,
        GRAY,
        BLUR,
        CANNY,
        INVERT,
        SOBEL
    };

    Mode process(int key);
};