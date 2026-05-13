#include "KeyProcessor.hpp"

KeyProcessor::Mode KeyProcessor::process(int key) {
    switch (key) {
        case '1': return ORIGINAL;
        case '2': return GRAY;
        case '3': return BLUR;
        case '4': return CANNY;
        case '5': return INVERT;
        case '6': return SOBEL;
        default: return ORIGINAL;
    }
}